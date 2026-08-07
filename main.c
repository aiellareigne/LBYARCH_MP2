#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

extern void imgCvtGrayFloatToInt(int height, int width, const float *src, uint8_t *dst);

void imgCvtGrayFloatToInt_C(int height, int width, const float *src, uint8_t *dst) {
    int total = height * width;
    for (int i = 0; i < total; i++) {
        float val = src[i] * 255.0f;
        int int_val = (int)lroundf(val);
        if (int_val < 0) int_val = 0;
        if (int_val > 255) int_val = 255;
        dst[i] = (uint8_t)int_val;
    }
}

int verify_correctness(int size, const uint8_t *asm_out, const uint8_t *c_out) {
    for (int i = 0; i < size; i++) {
        if (abs((int)asm_out[i] - (int)c_out[i]) > 1) {
            printf("Mismatch at index %d: ASM=%u, C=%u\n", i, asm_out[i], c_out[i]);
            return 0; // Fail
        }
    }
    return 1; // Correct
}

void run_benchmark(int height, int width, int runs) {
    int total_pixels = height * width;
    float *src = (float *)malloc(total_pixels * sizeof(float));
    uint8_t *dst_asm = (uint8_t *)malloc(total_pixels * sizeof(uint8_t));
    uint8_t *dst_c = (uint8_t *)malloc(total_pixels * sizeof(uint8_t));

    for (int i = 0; i < total_pixels; i++) {
        src[i] = (float)rand() / (float)RAND_MAX;
    }

    imgCvtGrayFloatToInt(height, width, src, dst_asm);
    imgCvtGrayFloatToInt_C(height, width, src, dst_c);

    int is_correct = verify_correctness(total_pixels, dst_asm, dst_c);
    printf("Image Size: %dx%d (%d pixels)\n", height, width, total_pixels);
    printf("Correctness Check: %s\n", is_correct ? "PASSED [OK]" : "FAILED [X]");

    double total_time_sec = 0.0;
    for (int r = 0; r < runs; r++) {
        clock_t start = clock();
        imgCvtGrayFloatToInt(height, width, src, dst_asm);
        clock_t end = clock();

        total_time_sec += (double)(end - start) / CLOCKS_PER_SEC;
    }

    double avg_time_ms = (total_time_sec / runs) * 1000.0;
    printf("Average Execution Time (%d runs): %.4f ms\n", runs, avg_time_ms);
    printf("--------------------------------------------------\n");

    free(src);
    free(dst_asm);
    free(dst_c);
}

void run_sample_demo() {
    int height = 3, width = 4;
    float sample_input[] = {
        0.25f, 0.35f, 0.45f, 0.33f,
        0.55f, 0.65f, 0.75f, 0.33f,
        0.85f, 0.95f, 0.15f, 0.33f
    };
    uint8_t sample_output[12];

    imgCvtGrayFloatToInt(height, width, sample_input, sample_output);

    printf("=== Example Demo (%dx%d) ===\n", height, width);
    printf("Input Float Array:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%.2f ", sample_input[i * width + j]);
        }
        printf("\n");
    }

    printf("\nConverted uint8 Output Array:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%3u ", sample_output[i * width + j]);
        }
        printf("\n");
    }
    printf("--------------------------------------------------\n");
}

int main() {
    srand((unsigned int)time(NULL));

    run_sample_demo();

    printf("=== Performance Benchmarks (30 Runs Average) ===\n");
    run_benchmark(10, 10, 30);
    run_benchmark(100, 100, 30);
    run_benchmark(1000, 1000, 30);

    return 0;
}
