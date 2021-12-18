#include "lz4Import.h"
#include <stdio.h>   // For printf()
#include <string.h>  // For memcmp()
#include <stdlib.h>  // For exit()
#include "lz4.h"     // This is all that is required to expose the prototypes for basic compression and decompression.
#include "lz4frame.h"
extern "C" {
#include "pike.h"
}

void run_screaming(const char* message, const int code) {
    printf("%s %d\n", message, code);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CryptLz4(char* data, int size, char* outdata)
{
    // LZ4 provides a function that will tell you the maximum size of compressed output based on input data via LZ4_compressBound().
    const int max_dst_size = LZ4_compressBound(size);
    // That's all the information and preparation LZ4 needs to compress *src into *compressed_data.
    // Invoke LZ4_compress_default now with our size values and pointers to our memory locations.
    // Save the return value for error checking.
    const int compressed_data_size = LZ4_compress_default(data, outdata, size, max_dst_size);
    // Check return_value to determine what happened.
    if (compressed_data_size <= 0)
    {
        run_screaming("A 0 or negative result from LZ4_compress_default() indicates a failure trying to compress the data. ", 1);
        return -1;
    }
    if (compressed_data_size > 0)
        printf("We successfully compressed some data! Ratio: %.2f\n",
            (float)compressed_data_size / size);
    return compressed_data_size;
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API LZ4compressBound(int size)
{
    return LZ4_compressBound(size);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API DecryptLz4(char* data, int size, char* outdata, int dstSize)
{
    const int decompressed_size = LZ4_decompress_safe(data, outdata, size, dstSize);
    if (decompressed_size < 0)
    {
        run_screaming("A negative result from LZ4_decompress_safe indicates a failure trying to decompress the data.  See exit code (echo $?) for value returned.", decompressed_size);
        return -1;
    }
    if (decompressed_size >= 0)
        printf("We successfully decompressed some data!\n");
    return decompressed_size;
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CryptLz43(char* data, int size, char* outdata)
{
    // LZ4 provides a function that will tell you the maximum size of compressed output based on input data via LZ4_compressBound().
    LZ4F_decompressionContext_t ctx = NULL;
    LZ4F_frameInfo_t info;
    LZ4F_errorCode_t code;
    size_t t_size = size;
    const int max_dst_size = LZ4_compressBound(size);

    size_t t_size2 = max_dst_size;
    code = LZ4F_createDecompressionContext(&ctx, LZ4F_VERSION);
    int decompressed_size = LZ4F_compressFrame(outdata, t_size2, data, t_size, NULL);

    return decompressed_size;
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API DecryptLz43(char* data, int size, char* outdata, int dstSize)
{
    LZ4F_decompressionContext_t ctx = NULL;
    LZ4F_frameInfo_t info;
    LZ4F_errorCode_t code;

    code = LZ4F_createDecompressionContext(&ctx, LZ4F_VERSION);
    size_t t_size = size;
    size_t t_dstSize = dstSize;
    int decompressed_size = LZ4F_decompress(ctx, data, &t_size, outdata, &t_dstSize, NULL);
    int a = 0;
    a++;
    return decompressed_size;
}

static Ctx** g_ctxs = NULL;
static int g_ctx_len = 0;
static int g_ctx_index = 0;

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CtxReInit(int ctx_id, int sd)
{
    if (ctx_id < 0 || ctx_id > g_ctx_index || g_ctxs == NULL)
    {
        return;
    }
    Ctx* ctx = g_ctxs[ctx_id];
    if (ctx == NULL)
        return;
    ctx_init(sd, ctx);
}

extern "C" int UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CtxInit(int sd)
{
    if (g_ctxs == NULL)
    {
        g_ctx_len = 2;
        g_ctxs = new Ctx * [g_ctx_len];
        memset(g_ctxs, 0, sizeof(Ctx*) * g_ctx_len);
        g_ctx_index = -1;
    }

    if (g_ctx_index >= g_ctx_len)
    {
        Ctx** pre_g_ctxs = g_ctxs;
        g_ctxs = new Ctx * [g_ctx_len * 2];
        g_ctx_len = g_ctx_len * 2;
        memset(g_ctxs, 0, sizeof(Ctx*) * g_ctx_len);
        memcpy(g_ctxs, pre_g_ctxs, sizeof(Ctx*) * g_ctx_len);
        delete pre_g_ctxs;
    }
    g_ctx_index++;
    Ctx* ctx = new Ctx();
    ctx_init(sd, ctx);
    g_ctxs[g_ctx_index] = ctx;
    return g_ctx_index;
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CtxDelete(int ctx_id)
{
    if (ctx_id < 0 || ctx_id > g_ctx_index || g_ctxs == NULL)
    {
        return;
    }
    Ctx* ctx = g_ctxs[ctx_id];
    if (ctx == NULL)
        return;
    delete ctx;
    g_ctxs[ctx_id] = NULL;
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CtxFree()
{
    if (g_ctxs == NULL)
    {
        return;
    }
    for (int i = 0; i <= g_ctx_index; ++i)
    {
        Ctx* ctx = g_ctxs[i];
        if (ctx != NULL)
        {
            delete ctx;
            g_ctxs[i] = NULL;
        }
    }
    delete[] g_ctxs;
    g_ctxs = NULL;
    g_ctx_len = 0;
    g_ctx_index = -1;
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CtxEncode(int ctx_id, void* buffer, int len)
{
    if (ctx_id < 0 || ctx_id > g_ctx_index || g_ctxs == NULL)
    {
        return;
    }
    Ctx* ctx = g_ctxs[ctx_id];
    if (ctx == NULL)
        return;
    ctx_encode(ctx, buffer, len);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CtxDecode(int ctx_id, void* buffer, int len)
{
    if (ctx_id < 0 || ctx_id > g_ctx_index || g_ctxs == NULL)
    {
        return;
    }
    Ctx* ctx = g_ctxs[ctx_id];
    if (ctx == NULL)
        return;
    ctx_decode(ctx, buffer, len);
}