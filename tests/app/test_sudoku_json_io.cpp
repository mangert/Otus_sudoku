#include "sudoku_json_io.h"
#include "../test_utils.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

namespace {

class TemporaryPath final {
public:
    explicit TemporaryPath(const std::string& suffix = ".json") {
        std::error_code error;
        auto directory = std::filesystem::temp_directory_path(error);
        if (error) {
            directory = ".";
        }

        const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = directory / ("otus_sudoku_json_" + std::to_string(timestamp) + suffix);
    }

    ~TemporaryPath() {
        std::error_code error;
        std::filesystem::remove(path_, error);
    }

    TemporaryPath(const TemporaryPath&) = delete;
    TemporaryPath& operator=(const TemporaryPath&) = delete;

    const std::filesystem::path& get() const {
        return path_;
    }

    bool write(const std::string& content) const {
        std::ofstream file(path_, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        file << content;
        return static_cast<bool>(file);
    }

private:
    std::filesystem::path path_;
};

template<size_t N>
bool readReturnsError(const std::string& content, SudokuIO::JsonIoError::Code expected_code) {
    TemporaryPath file;
    if (!file.write(content)) {
        return false;
    }

    auto result = SudokuIO::readBoardFromJsonFile<N>(file.get().string());
    return !result.has_value() && result.error().code == expected_code;
}

bool test_read_valid_4x4() {
    TemporaryPath file;
    ASSERT_TRUE(file.write(R"({
        "size": 4,
        "board": [
            [1, 0, 0, 4],
            [0, 4, 1, 0],
            [0, 1, 4, 0],
            [4, 0, 0, 1]
        ]
    })"));

    auto result = SudokuIO::readBoardFromJsonFile<4>(file.get().string());
    ASSERT_TRUE(result.has_value());

    SudokuIO::Board<4> expected = {{
        {{1, 0, 0, 4}},
        {{0, 4, 1, 0}},
        {{0, 1, 4, 0}},
        {{4, 0, 0, 1}}
    }};
    ASSERT_EQ(result.value(), expected);
    return true;
}

bool test_read_reordered_fields_and_whitespace() {
    TemporaryPath file;
    const std::string content =
        "\n\t{\n"
        "\t\t\"board\" : [[1,2,3,4], [3,4,1,2], [2,1,4,3], [4,3,2,1]],\n"
        "\t\t\"size\" : 4\n"
        "\t}\n";
    ASSERT_TRUE(file.write(content));

    auto result = SudokuIO::readBoardFromJsonFile<4>(file.get().string());
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->at(3).at(2), 2);
    return true;
}

bool test_round_trip_9x9() {
    SudokuIO::Board<9> board{};
    for (size_t row = 0; row < board.size(); ++row) {
        for (size_t col = 0; col < board[row].size(); ++col) {
            board[row][col] = static_cast<int>((row * 3 + row / 3 + col) % 9 + 1);
        }
    }

    TemporaryPath file;
    auto write_result = SudokuIO::writeBoardToJsonFile(file.get().string(), board);
    ASSERT_TRUE(write_result.has_value());

    auto read_result = SudokuIO::readBoardFromJsonFile<9>(file.get().string());
    ASSERT_TRUE(read_result.has_value());
    ASSERT_EQ(read_result.value(), board);

    std::ifstream saved(file.get(), std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(saved)), std::istreambuf_iterator<char>());
    ASSERT_TRUE(content.find("\"size\": 9") < content.find("\"board\""));
    ASSERT_TRUE(!content.empty() && content.back() == '\n');
    return true;
}

bool test_round_trip_16x16() {
    SudokuIO::Board<16> board{};
    board[0][0] = 16;
    board[15][15] = 1;

    TemporaryPath file;
    ASSERT_TRUE(SudokuIO::writeBoardToJsonFile(file.get().string(), board).has_value());

    auto result = SudokuIO::readBoardFromJsonFile<16>(file.get().string());
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value(), board);
    return true;
}

bool test_file_errors() {
    TemporaryPath missing;
    auto read_result = SudokuIO::readBoardFromJsonFile<4>(missing.get().string());
    ASSERT_FALSE(read_result.has_value());
    ASSERT_EQ(read_result.error().code, SudokuIO::JsonIoError::Code::FILE_OPEN_FAILED);

    std::error_code error;
    auto missing_directory = std::filesystem::temp_directory_path(error) /
        ("otus_sudoku_missing_" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    auto write_result = SudokuIO::writeBoardToJsonFile(
        (missing_directory / "board.json").string(), SudokuIO::Board<4>{});
    ASSERT_FALSE(write_result.has_value());
    ASSERT_EQ(write_result.error().code, SudokuIO::JsonIoError::Code::FILE_OPEN_FAILED);
    return true;
}

bool test_invalid_syntax() {
    using Code = SudokuIO::JsonIoError::Code;
    ASSERT_TRUE(readReturnsError<4>("", Code::INVALID_SYNTAX));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]])",
        Code::INVALID_SYNTAX));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],]})",
        Code::INVALID_SYNTAX));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]} tail)",
        Code::INVALID_SYNTAX));
    return true;
}

bool test_invalid_schema() {
    using Code = SudokuIO::JsonIoError::Code;
    const std::string board = R"([[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]])";

    ASSERT_TRUE(readReturnsError<4>("{\"size\":4}", Code::INVALID_SCHEMA));
    ASSERT_TRUE(readReturnsError<4>(
        "{\"size\":4,\"size\":4,\"board\":" + board + "}", Code::INVALID_SCHEMA));
    ASSERT_TRUE(readReturnsError<4>(
        "{\"size\":4,\"board\":" + board + ",\"name\":1}", Code::INVALID_SCHEMA));
    return true;
}

bool test_size_and_shape_errors() {
    using Code = SudokuIO::JsonIoError::Code;

    TemporaryPath size_file;
    ASSERT_TRUE(size_file.write(
        R"({"size":9,"board":[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})"));
    auto size_result = SudokuIO::readBoardFromJsonFile<4>(size_file.get().string());
    ASSERT_FALSE(size_result.has_value());
    ASSERT_EQ(size_result.error().code, Code::SIZE_MISMATCH);
    ASSERT_EQ(size_result.error().actual_size, 9);

    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_BOARD_SHAPE));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_BOARD_SHAPE));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_BOARD_SHAPE));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[0,0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_BOARD_SHAPE));
    return true;
}

bool test_invalid_cell_values() {
    using Code = SudokuIO::JsonIoError::Code;
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[["1",0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_CELL_VALUE));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[1.0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_CELL_VALUE));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[-1,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_CELL_VALUE));
    ASSERT_TRUE(readReturnsError<4>(
        R"({"size":4,"board":[[5,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]})",
        Code::INVALID_CELL_VALUE));

    SudokuIO::Board<4> invalid_board{};
    invalid_board[0][0] = -1;
    TemporaryPath file;
    auto write_result = SudokuIO::writeBoardToJsonFile(file.get().string(), invalid_board);
    ASSERT_FALSE(write_result.has_value());
    ASSERT_EQ(write_result.error().code, Code::INVALID_CELL_VALUE);
    return true;
}

} // namespace

int main() {
    TestRunner runner;
    runner.addTest("Read valid JSON 4x4", test_read_valid_4x4);
    runner.addTest("Read reordered JSON fields", test_read_reordered_fields_and_whitespace);
    runner.addTest("Round trip JSON 9x9", test_round_trip_9x9);
    runner.addTest("Round trip JSON 16x16", test_round_trip_16x16);
    runner.addTest("JSON file errors", test_file_errors);
    runner.addTest("Reject invalid JSON syntax", test_invalid_syntax);
    runner.addTest("Reject invalid JSON schema", test_invalid_schema);
    runner.addTest("Reject invalid JSON size and shape", test_size_and_shape_errors);
    runner.addTest("Reject invalid JSON cell values", test_invalid_cell_values);
    return runner.runAll() ? 0 : 1;
}
