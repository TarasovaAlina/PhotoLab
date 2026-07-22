NAME_PROJECT := PhotoLab

BUILD_DIR := build
GTEST_BUILD_DIR := $(BUILD_DIR)/gtest

COVERAGE_INFO := $(GTEST_BUILD_DIR)/coverage.info
FILTERED_INFO := $(GTEST_BUILD_DIR)/coverage_filtered.info
COVERAGE_DIR := $(GTEST_BUILD_DIR)/coverage

all: install

install:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

tests:
	cmake -S . -B $(GTEST_BUILD_DIR)
	cmake --build $(GTEST_BUILD_DIR)
	ctest --test-dir $(GTEST_BUILD_DIR) --output-on-failure

gcov_report:
	find $(GTEST_BUILD_DIR) -name "*.gcda" -delete
	find $(GTEST_BUILD_DIR) -name "*.gcov" -delete

	cmake -S . -B $(GTEST_BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(GTEST_BUILD_DIR)

	ctest --test-dir $(GTEST_BUILD_DIR) --output-on-failure

	lcov --capture \
		--directory $(GTEST_BUILD_DIR) \
		--output-file $(COVERAGE_INFO) \
		--ignore-errors mismatch

	lcov --extract \
		$(COVERAGE_INFO) \
		"*/CNN/*.cpp" \
		"*/Filters/*.cpp" \
		"*/include/CNN/*.h" \
		"*/include/Filters/*.h" \
		"*/include/reader_class.h" \
		--output-file $(FILTERED_INFO)

	genhtml \
		$(FILTERED_INFO) \
		--output-directory $(COVERAGE_DIR)

# создание документации
dvi:
	doxygen Doxyfile
	@echo -e "\nДля просмотра отчета откройте файл ./docs/html/index.html"

# Создание архива с программой
dist:
	mkdir -p $(NAME_PROJECT)
	cp -r photoLab include ui $(NAME_PROJECT)/.
	cp Makefile CMakeLists.txt main.cpp $(NAME_PROJECT)/.
	tar -czf Team_CPP8_$(NAME_PROJECT)_v2.2.tar.gz $(NAME_PROJECT)
	rm -rf $(NAME_PROJECT)

# удаление приложения
uninstall:
	rm -rf build *.json

clean:
	rm -rf *.o *.out .clang-format *.gz Testing docs
