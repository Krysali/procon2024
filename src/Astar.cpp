#include <iostream>
#include "torch/script.h"
#include "torch/torch.h"

// int main() {
//     torch::jit::script::Module model = torch::jit::load("/home/ubuntu/procon2024/src/models/pro_model.pt");
//     torch::Tensor x = torch::randn({1, 8, 256, 256});
//     std::cout << x;
//     return 0;
// }

int main() {
    torch::jit::script::Module model = torch::jit::load("/home/ubuntu/procon2024/src/models/pro_model.pt");
    torch::Tensor x = torch::randn({1, 8, 256, 256});

    torch::Tensor x_flat = x.flatten();

    // Vector to store the float values
    std::vector<float> values_temp;

    // Iterate over the flattened tensor and extract float values
    for (int i = 0; i < x_flat.size(0); i++) {
        float f = x_flat[i].item<float>(); // Convert tensor value to float
        values_temp.push_back(f);          // Add to the vector
    }

    // Print the first 5 values (just for checking)
    for (int i = 0; i < 5; i++) {
        std::cout << values_temp[i] << std::endl;
    }

    return 0;
}
