#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 600
#define HEIGHT 600

GLFWwindow *window;
VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice;
VkDevice device;
VkQueue queue;

void initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", NULL, NULL);
}

void createInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Super Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    const char *glfwExtensions[] = {"VK_KHR_surface", "VK_KHR_win32_surface"};

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = sizeof(glfwExtensions) / sizeof(glfwExtensions[0]);
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkResult instance_result = vkCreateInstance(&createInfo, NULL, &instance);

    if (instance_result != VK_SUCCESS)
    {
        printf("Vulkan instance creation failed!");
        getchar();
        exit(EXIT_FAILURE);
    }
}

void createSurface() {
    VkResult res = glfwCreateWindowSurface(instance, window, NULL, &surface);

    if (res != VK_SUCCESS) {
        printf("failed to create window surface!");
		getchar();
		exit(EXIT_FAILURE);
    }
}

void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    
    if (deviceCount == 0) {
        printf("failed to find GPUs with Vulkan support!");
		getchar();
		exit(EXIT_FAILURE);
    }

    VkPhysicalDevice *devices = malloc(deviceCount * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
    physicalDevice = devices[0];
    free(devices);
}

void createLogicalDevice() {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    const char* deviceExtensions[] = {"VK_KHR_swapchain"};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions;
    createInfo.enabledExtensionCount = 1;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateDevice(physicalDevice, &createInfo, NULL, &device);

    if (result != VK_SUCCESS) {
        printf("failed to create logical device!");
		getchar();
		exit(EXIT_FAILURE);
    }

    vkGetDeviceQueue(device, 0, 0, &queue);
}

void initVulkan()
{
    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}

void mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

int main(int argc, char **argv)
{
    initWindow();
    initVulkan();
    mainLoop();

    return 0;
}
