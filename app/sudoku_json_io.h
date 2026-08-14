#pragma once

#include <array>
#include <charconv>
#include <expected>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>

namespace SudokuIO {

template<size_t N>
using Board = std::array<std::array<int, N>, N>;

struct JsonIoError {
    enum class Code {
        FILE_OPEN_FAILED,
        FILE_READ_FAILED,
        INVALID_SYNTAX,
        INVALID_SCHEMA,
        SIZE_MISMATCH,
        INVALID_BOARD_SHAPE,
        INVALID_CELL_VALUE,
        FILE_WRITE_FAILED
    };

    Code code;
    int actual_size = 0;
};

namespace detail {

inline std::unexpected<JsonIoError> makeJsonError(
    JsonIoError::Code code, int actual_size = 0) {
    return std::unexpected(JsonIoError{ code, actual_size });
}

template<size_t N>
class JsonBoardParser final {
public:
    explicit JsonBoardParser(std::string_view input)
        : input_(input) {}

    std::expected<Board<N>, JsonIoError> parse() {
        if (!consume('{')) {
            return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
        }

        Board<N> board{};
        bool has_size = false;
        bool has_board = false;

        skipWhitespace();
        if (peek('}')) {
            return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
        }

        while (true) {
            auto key = parseKey();
            if (!key.has_value()) {
                return std::unexpected(key.error());
            }
            if (!consume(':')) {
                return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
            }

            if (key.value() == "size") {
                if (has_size) {
                    return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
                }

                auto size = parseInteger(JsonIoError::Code::INVALID_SCHEMA);
                if (!size.has_value()) {
                    return std::unexpected(size.error());
                }
                has_size = true;

                if (size.value() != static_cast<int>(N)) {
                    return makeJsonError(JsonIoError::Code::SIZE_MISMATCH, size.value());
                }
            }
            else if (key.value() == "board") {
                if (has_board) {
                    return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
                }

                auto parsed_board = parseBoard();
                if (!parsed_board.has_value()) {
                    return std::unexpected(parsed_board.error());
                }
                board = parsed_board.value();
                has_board = true;
            }
            else {
                return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
            }

            skipWhitespace();
            if (consume('}')) {
                break;
            }
            if (!consume(',')) {
                return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
            }

            skipWhitespace();
            if (peek('}')) {
                return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
            }
        }

        if (!has_size || !has_board) {
            return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
        }

        skipWhitespace();
        if (position_ != input_.size()) {
            return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
        }

        return board;
    }

private:
    void skipWhitespace() {
        while (position_ < input_.size()) {
            const char current = input_[position_];
            if (current != ' ' && current != '\t' && current != '\r' && current != '\n') {
                break;
            }
            ++position_;
        }
    }

    bool peek(char expected) {
        skipWhitespace();
        return position_ < input_.size() && input_[position_] == expected;
    }

    bool consume(char expected) {
        skipWhitespace();
        if (position_ >= input_.size() || input_[position_] != expected) {
            return false;
        }
        ++position_;
        return true;
    }

    std::expected<std::string_view, JsonIoError> parseKey() {
        skipWhitespace();
        if (position_ >= input_.size() || input_[position_] != '"') {
            return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
        }
        ++position_;

        const size_t key_start = position_;
        while (position_ < input_.size() && input_[position_] != '"') {
            const unsigned char current = static_cast<unsigned char>(input_[position_]);
            if (current < 0x20 || input_[position_] == '\\') {
                return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
            }
            ++position_;
        }

        if (position_ >= input_.size()) {
            return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
        }

        const auto key = input_.substr(key_start, position_ - key_start);
        ++position_;
        return key;
    }

    std::expected<int, JsonIoError> parseInteger(JsonIoError::Code error_code) {
        skipWhitespace();
        if (position_ >= input_.size() || input_[position_] < '0' || input_[position_] > '9') {
            return makeJsonError(error_code);
        }

        const size_t number_start = position_;
        if (input_[position_] == '0') {
            ++position_;
            if (position_ < input_.size() && input_[position_] >= '0' && input_[position_] <= '9') {
                return makeJsonError(error_code);
            }
        }
        else {
            while (position_ < input_.size() &&
                input_[position_] >= '0' && input_[position_] <= '9') {
                ++position_;
            }
        }

        if (position_ < input_.size()) {
            const char suffix = input_[position_];
            if (suffix == '.' || suffix == 'e' || suffix == 'E' || suffix == '+' || suffix == '-') {
                return makeJsonError(error_code);
            }
        }

        int value = 0;
        const char* first = input_.data() + number_start;
        const char* last = input_.data() + position_;
        const auto conversion = std::from_chars(first, last, value);
        if (conversion.ec != std::errc{} || conversion.ptr != last) {
            return makeJsonError(error_code);
        }
        return value;
    }

    std::expected<std::array<int, N>, JsonIoError> parseRow() {
        if (!consume('[')) {
            return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
        }

        std::array<int, N> row{};
        for (size_t col = 0; col < N; ++col) {
            if (col > 0) {
                if (peek(']')) {
                    return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
                }
                if (!consume(',')) {
                    return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
                }
            }

            auto value = parseInteger(JsonIoError::Code::INVALID_CELL_VALUE);
            if (!value.has_value()) {
                return std::unexpected(value.error());
            }
            if (value.value() < 0 || value.value() > static_cast<int>(N)) {
                return makeJsonError(JsonIoError::Code::INVALID_CELL_VALUE);
            }
            row[col] = value.value();
        }

        skipWhitespace();
        if (peek(',')) {
            consume(',');
            if (peek(']')) {
                return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
            }
            return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
        }
        if (!consume(']')) {
            return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
        }

        return row;
    }

    std::expected<Board<N>, JsonIoError> parseBoard() {
        if (!consume('[')) {
            return makeJsonError(JsonIoError::Code::INVALID_SCHEMA);
        }

        Board<N> board{};
        for (size_t row = 0; row < N; ++row) {
            if (row > 0) {
                if (peek(']')) {
                    return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
                }
                if (!consume(',')) {
                    return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
                }
            }

            auto parsed_row = parseRow();
            if (!parsed_row.has_value()) {
                return std::unexpected(parsed_row.error());
            }
            board[row] = parsed_row.value();
        }

        skipWhitespace();
        if (peek(',')) {
            consume(',');
            if (peek(']')) {
                return makeJsonError(JsonIoError::Code::INVALID_SYNTAX);
            }
            return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
        }
        if (!consume(']')) {
            return makeJsonError(JsonIoError::Code::INVALID_BOARD_SHAPE);
        }

        return board;
    }

    std::string_view input_;
    size_t position_ = 0;
};

} // namespace detail

/**
 * @brief Читает доску судоку из JSON-файла строгого формата.
 * @tparam N размер стороны доски
 * @param filename имя JSON-файла
 * @return доска либо описание ошибки чтения и валидации
 */
template<size_t N>
std::expected<Board<N>, JsonIoError> readBoardFromJsonFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return detail::makeJsonError(JsonIoError::Code::FILE_OPEN_FAILED);
    }

    const std::string content(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (file.bad()) {
        return detail::makeJsonError(JsonIoError::Code::FILE_READ_FAILED);
    }

    return detail::JsonBoardParser<N>(content).parse();
}

/**
 * @brief Записывает доску судоку в JSON-файл строгого формата.
 * @tparam N размер стороны доски
 * @param filename имя JSON-файла
 * @param board доска для сохранения
 * @return успех либо описание ошибки записи и валидации
 */
template<size_t N>
std::expected<void, JsonIoError> writeBoardToJsonFile(
    const std::string& filename, const Board<N>& board) {
    for (const auto& row : board) {
        for (int value : row) {
            if (value < 0 || value > static_cast<int>(N)) {
                return detail::makeJsonError(JsonIoError::Code::INVALID_CELL_VALUE);
            }
        }
    }

    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return detail::makeJsonError(JsonIoError::Code::FILE_OPEN_FAILED);
    }

    file << "{\n";
    file << "  \"size\": " << N << ",\n";
    file << "  \"board\": [\n";
    for (size_t row = 0; row < N; ++row) {
        file << "    [";
        for (size_t col = 0; col < N; ++col) {
            file << board[row][col];
            if (col + 1 < N) {
                file << ", ";
            }
        }
        file << "]";
        if (row + 1 < N) {
            file << ",";
        }
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";

    if (!file) {
        return detail::makeJsonError(JsonIoError::Code::FILE_WRITE_FAILED);
    }
    return {};
}

} // namespace SudokuIO
