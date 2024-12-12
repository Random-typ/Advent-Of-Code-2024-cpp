
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda/atomic>

#include "../Advent Of Code 2024 cpp/src/util.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>


struct Numbers {
    int64_t count;
    int64_t numbers;

    __device__ int64_t at(size_t index) {
        return (&numbers)[index];
    }
};



__device__ int64_t computeCuda(int64_t _a, int64_t _b, char _operator) {
    switch (_operator) {
    case '-':
        return _a - _b;
    case '+':
        return _a + _b;
    case '*':
        return _a * _b;
    case '/':
        return _a / _b;
    case '|':
        int multiplier = 1;
        while (multiplier <= _b) {
            multiplier *= 10;
        }
        return _a * multiplier + _b;
    }
}

__global__ void day7ComputeCuda(uint64_t* _sums, Numbers* _numbers, int64_t* _offsets, const char* _operators, int* _completeCounter) {
    int thI = threadIdx.x;
    int operatorCount = 0;
    while (_operators[operatorCount] != '\0')
    {
        operatorCount++;
    }

    int c = _numbers->count;
    _numbers = (Numbers*)(((char*)(_numbers)) + _offsets[thI]);
    c = _numbers->count;
    for (int64_t i = 0; i <= pow(operatorCount, _numbers->count - 1); i++)
    {
        int64_t result = _numbers->at(1);
        for (size_t pos = 2, j = i; pos < _numbers->count; pos++)
        {
            result = computeCuda(result, _numbers->at(pos), _operators[j % operatorCount]);
            j /= operatorCount;
        }
        if (result == _numbers->at(0))
        {
            _sums[thI] = _numbers->at(0);
            break;
        }
    }
    printf("Finished\n");
}

void day7Cuda(const std::vector<std::string>& _calibrations, std::string _operators) {
    std::vector<char> numbers;
    std::vector<int64_t> offsets = { 0 };
    int64_t lastCount = 0;
    int64_t nextFree = sizeof(int64_t);
    std::regex r("(\\d*)");

    uint64_t calibrationCount = 0;
    for (auto& i : _calibrations)
    {
        if (i.empty())
        {
            continue;
        }
        calibrationCount++;
        uint64_t count = 0;
        for (std::sregex_iterator j = std::sregex_iterator(i.begin(), i.end(), r), end = std::sregex_iterator(); j != end; j++)
        {
            if (!j->str().empty())
            {
                if (numbers.size() <= nextFree + sizeof(int64_t))
                {
                    numbers.resize(numbers.size() + 512, '\0');
                }
                *(int64_t*)(numbers.data() + nextFree) = std::stoll(j->str());
                nextFree += sizeof(int64_t);
                count++;
            }
        }
        if (numbers.size() <= lastCount + sizeof(int64_t))
        {
            numbers.resize(numbers.size() + 512, '\0');
        }
        *(int64_t*)(numbers.data() + lastCount) = count;
        lastCount = nextFree;
        offsets.emplace_back(lastCount);
        nextFree += sizeof(int64_t);
    }

    cudaSetDevice(0);

    const char* operators_dev;
    uint64_t* sums_dev;
    Numbers* numbers_dev;
    int64_t* offsets_dev;

    cudaMalloc(&operators_dev, _operators.size());
    cudaMalloc(&sums_dev, calibrationCount * sizeof(uint64_t));
    cudaMalloc(&numbers_dev, numbers.size());
    cudaMalloc(&offsets_dev, offsets.size() * sizeof(uint64_t));

    cudaMemcpy((void*)operators_dev, _operators.data(), _operators.size(), cudaMemcpyHostToDevice);
    cudaMemset(sums_dev, 0, calibrationCount * sizeof(uint64_t));
    cudaMemcpy((void*)numbers_dev, numbers.data(), numbers.size(), cudaMemcpyHostToDevice);
    cudaMemcpy((void*)offsets_dev, offsets.data(), offsets.size() * sizeof(uint64_t), cudaMemcpyHostToDevice);
    
    int completeCount = 0;
    int* completeCount_dev;
    cudaMalloc(&completeCount_dev, sizeof(int));
    cudaMemcpy(completeCount_dev, &completeCount, sizeof(int), cudaMemcpyHostToDevice);


    day7ComputeCuda<<<1, calibrationCount>>>(sums_dev, numbers_dev, offsets_dev, operators_dev, completeCount_dev);

    cudaError_t cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
    }

    cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
    }

    std::vector<int64_t> sums;
    sums.resize(calibrationCount, 0);
    cudaMemcpy(sums.data(), sums_dev, sums.size() * sizeof(int64_t), cudaMemcpyDeviceToHost);
    
    uint64_t sum = 0;
    for (auto& i : sums)
    {
        sum += i;
    }

    cudaFree((void*)operators_dev);
    cudaFree(sums_dev);
    cudaFree(numbers_dev);
    cudaFree(offsets_dev);

    std::cout << "Day7 Part2: " << sum;

}

cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);

__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
} 

int main()
{
    std::vector<std::string> calibrations = serializeInput(readInput(7));

    day7Cuda(calibrations, "-+*/|");
    const int arraySize = 5;
    const int a[arraySize] = { 1, 2, 3, 4, 5 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return cudaStatus;
}
