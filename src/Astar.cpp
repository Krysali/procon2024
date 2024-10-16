#include <iostream>
#include "torch/script.h"
#include "torch/torch.h"

int main() {
    torch::jit::script::Module model = torch::jit::load("../models/pro_model.pt");
    torch::Tensor x = torch::randn({1, 8, 256, 256});
    std::cout << x;
    return 0;
}
