#include <filesystem>
#include <set>
#include <map>
#include <fmt/format.h>
#include <iostream>
#include <range/v3/all.hpp>
#include <cassert>
#include <fstream>

int main() {
    constexpr auto TRAINING_SIZE = 0.7;
    constexpr auto TEST_SIZE = 0.2;
    constexpr auto VALIDATION_SIZE = 1.0 - TRAINING_SIZE - TEST_SIZE;

    const std::filesystem::path sourceDir = R"(C:\Users\julian.GTI\Desktop\GTSRB\Final_Training\Images)";
    const std::filesystem::path targetDir = R"(C:\Users\julian.GTI\Desktop\Results)";

    const std::filesystem::path trainTxtPath = R"(C:\Users\julian.GTI\Desktop\train.txt)";
    const std::filesystem::path testTxtPath = R"(C:\Users\julian.GTI\Desktop\test.txt)";
    const std::filesystem::path valTxtPath = R"(C:\Users\julian.GTI\Desktop\val.txt)";

    std::map<int, std::vector<std::filesystem::path>> dataByClasses;

    for(const auto& classFolder : std::filesystem::directory_iterator(sourceDir)) {
        const auto classString = classFolder.path().filename();
        const auto classId = std::stoi(classString);

        for(const auto& data : std::filesystem::directory_iterator(classFolder)) {
            if(data.path().extension() == ".ppm") {
                const auto imageName = fmt::format("class{}Image{}.jpg", classId, data.path().stem().string());
                const auto imagePath = targetDir / imageName;
                dataByClasses[classId].push_back(imagePath);
            }
        }
    }

    std::ofstream trainFileStream(trainTxtPath);
    std::ofstream testFileStream(testTxtPath);
    std::ofstream valFileStream(valTxtPath);

    for(auto [classId, images] : dataByClasses) {
        ranges::shuffle(images);

        const auto trainingSize = static_cast<int>(images.size() * TRAINING_SIZE);
        const auto testSize = static_cast<int>(images.size() * TEST_SIZE);
        const auto validationSize = static_cast<int>(images.size() * VALIDATION_SIZE);
        const auto totalSize = trainingSize + testSize + validationSize;
        assert(totalSize <= images.size());

        const auto trainBegin = 0;
        const auto trainEnd = trainBegin + trainingSize;
        const auto testBegin = trainEnd;
        const auto testEnd = testBegin + testSize;
        const auto valBegin = testEnd;
        const auto valEnd = valBegin + validationSize;
        assert(totalSize == valEnd);

        for(const auto& path : images | ranges::views::slice(trainBegin, trainEnd)) {
            trainFileStream << path.string() << "\n";
        }

        for(const auto& path : images | ranges::views::slice(testBegin, testEnd)) {
            testFileStream << path.string() << "\n";
        }

        for(const auto& path : images | ranges::views::slice(valBegin, valEnd)) {
            valFileStream << path.string() << "\n";
        }

    }

}
