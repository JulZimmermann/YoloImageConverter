#include <iostream>
#include <filesystem>
#include <fstream>

#include <fmt/format.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <csv.h>

int main() {
    const std::filesystem::path sourceDir = R"(C:\Users\julian.GTI\Desktop\GTSRB\Final_Training\Images)";
    const std::filesystem::path targetDir = R"(C:\Users\julian.GTI\Desktop\Results)";

    std::filesystem::create_directories(targetDir);

    for(const auto& classFolder : std::filesystem::directory_iterator(sourceDir)) {
        const auto classString = classFolder.path().filename();
        const auto classId = std::stoi(classString);

        for(const auto& data : std::filesystem::directory_iterator(classFolder)) {
            if(data.path().extension() == ".ppm") {
                const auto imageName = fmt::format("class{}Image{}.jpg", classId, data.path().stem().string());

                const auto image = cv::imread(data.path().string());
                const auto targetPath = targetDir / imageName;

                cv::imwrite(targetPath.string(), image);
            }

            if(data.path().extension() == ".csv") {
                using CSVReader = io::CSVReader<7, io::trim_chars<' '>, io::no_quote_escape<';'>>;

                CSVReader csvReader(data.path().string());
                csvReader.read_header(io::ignore_extra_column, "Filename", "Width", "Height", "Roi.X1", "Roi.Y1", "Roi.X2", "Roi.Y2");

                std::string fileName;
                int imageWith, imageHeight, x1, x2, y1, y2;

                while(csvReader.read_row(fileName, imageWith, imageHeight, x1, y1, x2, y2)) {
                    const auto with = std::abs(x1 - x2);
                    const auto height = std::abs(y1 - y2);
                    const auto xMiddle = std::min(x1, x2) + with / 2;
                    const auto yMiddle = std::min(y1, y2) + height / 2;

                    const auto withRelative = static_cast<double>(with) / imageWith;
                    const auto heightRelative = static_cast<double>(height) / imageHeight;
                    const auto xMiddleRelative = static_cast<double>(xMiddle) / imageWith;
                    const auto yMiddleRelative = static_cast<double>(yMiddle) / imageHeight;

                    const auto line = fmt::format("{} {} {} {} {}", classId, xMiddleRelative, yMiddleRelative, withRelative, heightRelative);

                    const auto fileNameStem = std::filesystem::path(fileName).stem().string();
                    const auto dataName = fmt::format("class{}Image{}.txt", classId, fileNameStem);
                    const auto dataPath = targetDir / dataName;

                    std::ofstream ofs(dataPath);
                    ofs << line << std::endl;
                }
            }
        }

        std::cout << fmt::format("Finished directory {}", classFolder.path().string()) << std::endl;
    }

}
