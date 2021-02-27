#include <array>
#include <future>

#include "src/rtweekend.h"

#include "src/Color.h"
#include "src/Hittable_list.h"
#include "src/Sphere.h"
#include "src/Moving_sphere.h"
#include "src/Bvh.h"
#include "src/aarect.h"
#include "src/Box.h"
#include "src/Constant_medium.h"
#include "src/Pdf.h"

#include "src/Ray.h"
#include "src/Camera.h"
#include "src/Material.h"
#include "src/Scenes.h"

//makes it so, we can include both headers in whichever order we want, otherwise, glad goes first
//#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"

color ray_color( const ray& r, const color& background, const hittable& world,
                 std::shared_ptr<hittable_list>& lights, int depth ) 
{
    hit_record rec;
    // Recursion limit
    if (depth <= 0) return color(0, 0, 0);

    // Nothing hit, so it means background
    if (!world.hit(r, 0.001, infinity, rec)) return background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
            * ray_color(srec.specular_ray, background, world, lights, depth - 1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);

    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
        + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
        * ray_color(scattered, background, world, lights, depth - 1) / pdf_val;
}


int main() {

    constexpr int windowWidth = 640;
    constexpr int windowHeight = 480;

    GLFWwindow* window;

    if ( glfwInit() == 0 )  return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glfwSetErrorCallback([](int error, const char* description) {std::cerr << "Error " << error << ": " << description << '\n'; });

    window = glfwCreateWindow(windowWidth, windowHeight, "Hello Raytracing", NULL, NULL);
    if (window == NULL){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize OpenGL context" << '\n';
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

    // Image
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    color background(0, 0, 0);

    switch (6) {
    case 1:
        world = random_scene();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        break;

    case 2:
        world = two_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        break;
    case 3:
        world = two_perlin_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        break;
    case 4:
        world = earth();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        break;
    case 5:
        world = simple_light();
        background = color(0, 0, 0);
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        break;
    case 6:
        world = cornell_box();
        background = color(0, 0, 0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        break;
    case 7:
        world = cornell_smoke();
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        break;
    default:
    case 8:
        world = final_scene();
        background = color(0, 0, 0);
        lookfrom = point3(478, 278, -600);
        lookat = point3(278, 278, 0);
        break;
    }

    //World BCH and Lights
    bvh_node worldBVH{ world, 0, 1 };
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));

    // Camera
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render

    constexpr int coreCount = 6;
    std::atomic<int> scanLinesLeft = image_height;
    std::array<std::future<std::vector<color>>, coreCount> results;

    for (int thread = 0; thread < coreCount; ++thread) {
        results[thread] = std::async(std::launch::async, [&](int threadNum) {
            std::vector<color> buffer;

            auto range = image_height / coreCount;
            if (image_height % coreCount) ++range;

            auto start = image_height - 1 - threadNum * range;
            auto end = start - range + 1;
            if (threadNum == coreCount-1) end = 0;

            for (int j = start; j >= end; --j) {
                std::cerr << "\rScanlines remaining: " << --scanLinesLeft+1 << ' ' << std::flush;
                for (int i = 0; i < image_width; ++i) {
                    color pixel_color(0, 0, 0);
                    for (int s = 0; s < samples_per_pixel; ++s) {
                        auto u = (i + random_double()) / (image_width - 1);
                        auto v = (j + random_double()) / (image_height - 1);
                        ray r = cam.get_ray(u, v);
                        pixel_color += ray_color(r, background, world, lights, max_depth);
                    }
                    buffer.push_back(pixel_color);
                }
            }

            return buffer;
            }, thread);
    }

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (auto& result : results) {
        auto buffer = result.get();
        for(auto& color:buffer)
            write_color(std::cout, color, samples_per_pixel);
    }
       

    std::cerr << "\nDone.\n";
}