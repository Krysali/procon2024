#include <torch/torch.h>
#include "torch/script.h"
#include <vector>
#include <algorithm>
#include <iostream>

// Assuming you have a struct/class named Action
struct Action {
    int die;
    int x;
    int y;
    int dir;

    Action(int d, int x_val, int y_val, int dr) : die(d), x(x_val), y(y_val), dir(dr) {}
};


std::vector<Action> topK(torch::jit::script::Module& model, torch::Tensor input_tensor, int K) {
    model.eval();

    torch::NoGradGuard no_grad; 
    auto output = model.forward({input_tensor}).toTuple()->elements();
    auto die_probs = output[0].toTensor();
    auto x_probs = output[1].toTensor();
    auto y_probs = output[2].toTensor();
    auto direction_probs = output[3].toTensor();

    // Get sizes for bounds checking later
    int die_size = die_probs.size(1);
    int x_size = x_probs.size(1);
    int y_size = y_probs.size(1);
    int dir_size = direction_probs.size(1);


    auto die_pro_idx = torch::sort(die_probs[0], /*descending=*/true);
    auto x_pro_idx = torch::sort(x_probs[0], /*descending=*/true);
    auto y_pro_idx = torch::sort(y_probs[0], /*descending=*/true);
    auto dir_pro_idx = torch::sort(direction_probs[0], /*descending=*/true);

    auto die_pro = std::get<0>(die_pro_idx);
    auto index_die = std::get<1>(die_pro_idx);
    auto x_pro = std::get<0>(x_pro_idx);
    auto index_x = std::get<1>(x_pro_idx);
    auto y_pro = std::get<0>(y_pro_idx);
    auto index_y = std::get<1>(y_pro_idx);
    auto dir_pro = std::get<0>(dir_pro_idx);
    auto index_dir = std::get<1>(dir_pro_idx);

    std::vector<std::pair<float, std::tuple<int, int, int, int>>> values;

    // Use a priority queue to efficiently keep track of the top K actions
    std::priority_queue<std::pair<float, std::tuple<int, int, int, int>>, std::vector<std::pair<float, std::tuple<int, int, int, int>>>, std::greater<std::pair<float, std::tuple<int, int, int, int>>>> pq;

    for (int die = 0; die < die_size; ++die) {
        for (int x = 0; x < x_size; ++x) {
            for (int y = 0; y < y_size; ++y) {
                for (int dir = 0; dir < dir_size; ++dir) {
                    float prob = die_pro[die].item<float>() * x_pro[x].item<float>() * y_pro[y].item<float>() * dir_pro[dir].item<float>();
                    if (pq.size() < K) {
                        pq.push({prob, {index_die[die].item<int>(), index_x[x].item<int>(), index_y[y].item<int>(), index_dir[dir].item<int>()}});
                    } else if (prob > pq.top().first) {
                        pq.pop();
                        pq.push({prob, {index_die[die].item<int>(), index_x[x].item<int>(), index_y[y].item<int>(), index_dir[dir].item<int>()}});
                    }
                }
            }
        }
    }


    while (!pq.empty()) {
        values.push_back(pq.top());
        pq.pop();
    }
    std::reverse(values.begin(), values.end()); // Since we used a min-heap, reverse for descending order

    std::vector<Action> res;
    for (int k = 0; k < K && k < values.size(); ++k) { // Guard against values being smaller than K
        res.push_back(Action(std::get<0>(values[k].second), std::get<1>(values[k].second), std::get<2>(values[k].second), std::get<3>(values[k].second)));
        std::cout << values[k].first << " " << std::get<0>(values[k].second) << " " << std::get<1>(values[k].second) << " " << std::get<2>(values[k].second) << " " << std::get<3>(values[k].second) << std::endl;
    }

    return res;
}

int main() {
    torch::jit::script::Module model = torch::jit::load("/home/ubuntu/procon2024/src/models/pro_model.pt");

    // Example usage: You need to provide a proper input tensor
    torch::Tensor input_tensor = torch::randn({1, 8, 256, 256});
    std::vector<Action> top_actions = topK(model, input_tensor, 10); 

    std::cout << "Top " << K << " actions:" << std::endl;

    for (const auto& action : top_actions) {
        std::cout << action << std::endl; 
    }

    return 0;
}