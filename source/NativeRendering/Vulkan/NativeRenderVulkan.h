#ifndef NRV__
#define NRV__
#define VK_NO_PROTOTYPES
#include "../Unity/IUnityGraphics.h"
#include "../Unity/IUnityGraphicsVulkan.h"
#include "../RenderAPI.h"
#include <string.h>
#include <map>
#include <vector>
#include <math.h>
#include <exception>
#include <optional>
#include <cstdint>
#define VULKAN_DEFINE_API_FUNCPTR(func) static PFN_##func func

#define UNITY_USED_VULKAN_API_FUNCTIONS(apply1) \
    apply1(vkCreateInstance); \
    apply1(vkCmdBeginRenderPass); \
    apply1(vkCreateBuffer); \
    apply1(vkGetPhysicalDeviceMemoryProperties); \
    apply1(vkGetBufferMemoryRequirements); \
    apply1(vkMapMemory); \
    apply1(vkBindBufferMemory); \
    apply1(vkAllocateMemory); \
    apply1(vkDestroyBuffer); \
    apply1(vkFreeMemory); \
    apply1(vkUnmapMemory); \
    apply1(vkQueueWaitIdle); \
    apply1(vkDeviceWaitIdle); \
    apply1(vkCmdCopyBufferToImage); \
    apply1(vkFlushMappedMemoryRanges); \
    apply1(vkCreatePipelineLayout); \
    apply1(vkCreateShaderModule); \
    apply1(vkDestroyShaderModule); \
    apply1(vkCreateGraphicsPipelines); \
    apply1(vkCmdBindPipeline); \
    apply1(vkCmdDraw); \
    apply1(vkCmdDrawIndexed); \
    apply1(vkCmdPushConstants); \
    apply1(vkCmdBindVertexBuffers); \
    apply1(vkCmdBindIndexBuffer); \
    apply1(vkDestroyPipeline); \
    apply1(vkGetPhysicalDeviceQueueFamilyProperties); \
    apply1(vkGetPhysicalDeviceSurfaceSupportKHR); \
    apply1(vkCreateCommandPool); \
    apply1(vkAllocateCommandBuffers); \
    apply1(vkBeginCommandBuffer); \
    apply1(vkCmdCopyBuffer); \
    apply1(vkEndCommandBuffer); \
    apply1(vkQueueSubmit); \
    apply1(vkFreeCommandBuffers); \
    apply1(vkDestroyPipelineLayout);


VULKAN_DEFINE_API_FUNCPTR(vkGetInstanceProcAddr);

UNITY_USED_VULKAN_API_FUNCTIONS(VULKAN_DEFINE_API_FUNCPTR);

#undef VULKAN_DEFINE_API_FUNCPTR



struct VulkanBuffer
{
    VkBuffer buffer;
    VkDeviceMemory deviceMemory;
    void* mapped;
    VkDeviceSize sizeInBytes;
    VkDeviceSize deviceMemorySize;
    VkMemoryPropertyFlags deviceMemoryFlags;
};

class VulkanMesh
{
public:
    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexBuffer;
    VulkanBuffer vk_vertexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkBuffer indexBuffer;
    VulkanBuffer vk_indexBuffer;
};

class RenderAPI_Vulkan : public RenderAPI
{
public:
    RenderAPI_Vulkan();
    virtual ~RenderAPI_Vulkan() { }

    virtual void ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces);
    virtual bool GetUsesReverseZ() { return true; }
    virtual void DrawSimpleTriangles(const float worldMatrix[16], int triangleCount, const void* verticesFloat3Byte4);
    virtual void* BeginModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int* outRowPitch);
    virtual void EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int rowPitch, void* dataPtr);
    virtual void* BeginModifyVertexBuffer(void* bufferHandle, size_t* outBufferSize);
    virtual void EndModifyVertexBuffer(void* bufferHandle);

private:
    typedef std::vector<VulkanBuffer> VulkanBuffers;
    typedef std::map<unsigned long long, VulkanBuffers> DeleteQueue;

protected:
    bool CreateVulkanBuffer(size_t bytes, VulkanBuffer* buffer, VkBufferUsageFlags usage);
    void ImmediateDestroyVulkanBuffer(const VulkanBuffer& buffer);
    void SafeDestroy(unsigned long long frameNumber, const VulkanBuffer& buffer);
    void GarbageCollect(bool force = false);

public:
    IUnityGraphicsVulkan* m_UnityVulkan;
    UnityVulkanInstance m_Instance;
    VulkanBuffer m_TextureStagingBuffer;
    VulkanBuffer m_VertexStagingBuffer;
    std::map<unsigned long long, VulkanBuffers> m_DeleteQueue;
    VkPipelineLayout m_TrianglePipelineLayout;
    VkPipeline m_TrianglePipeline;
    VkRenderPass m_TrianglePipelineRenderPass;
};

UnityVulkanInstance* G_VulkanInstance;

#define G_DEVICE (G_VulkanInstance->device)


class NativeRenderVulkan : public RenderAPI_Vulkan
{
public:
    virtual void DrawMesh(NativeMesh* mesh, const float* worldMatrix);
    VkPipelineLayout CreateTrianglePipelineLayout(VkDevice device);
    VkPipeline CreateTrianglePipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, VkPipelineCache pipelineCache);
    void createNativeMesh(NativeMesh* mesh);
    void createVertexBuffer(NativeMesh* mesh);
    void createIndexBuffer(NativeMesh* mesh);
    void createVertexBuffer2(NativeMesh* mesh);
    void createIndexBuffer2(NativeMesh* mesh);
    bool CreateVulkanBuffer(size_t sizeInBytes, VulkanBuffer* buffer, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    static int FindMemoryTypeIndex(VkPhysicalDeviceMemoryProperties const& physicalDeviceMemoryProperties, VkMemoryRequirements const& memoryRequirements, VkMemoryPropertyFlags memoryPropertyFlags);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};

#endif