#include <gtest/gtest.h>

#include <filesystem>

#include "Logging/Logging.h"
#include "Perceptron/Perceptron.h"

int main(int argc, char **argv) {
  std::filesystem::path currentPath = std::filesystem::absolute(argv[0]);
  std::filesystem::path parentFolder = currentPath.parent_path();
  std::filesystem::current_path(parentFolder);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class PerceptronTest : public ::testing::Test {
 protected:
  //    QTextEdit output_;
  //    s21::Logging logging_{nullptr};
  s21::ParseFile parser_;
  s21::Perceptron perceptron_;
  s21::GraphMLP p_graph_;
  s21::MatrixMLP p_matrix_;
  s21::DataNetwork data_config_{{784, 100, 100, 26}, 0.7, 0.1};
  s21::ParseFile::data_type data_;
};

TEST_F(PerceptronTest, ParcerTestGoodFile) {
  data_ = parser_.Parser("../../Tests/TestFiles/file1.csv");
  ASSERT_EQ(data_.size(), 20);
  for (int i = 0; i < (int)data_.size(); ++i) {
    ASSERT_EQ(data_[i].pixels.size(), 784);
    ASSERT_EQ(data_[i].answer, 20 - i);
  }
}

TEST_F(PerceptronTest, ParcerTestBadRead) {
  ASSERT_ANY_THROW(parser_.Parser("../../Tests/TestFiles/file2.csv"));
}

TEST_F(PerceptronTest, ParcerTestBadFile) {
  data_ = parser_.Parser("../../Tests/TestFiles/nofile.csv");
  ASSERT_EQ(data_.size(), 0);
}

TEST_F(PerceptronTest, MatrixType) {
  ASSERT_EQ(p_matrix_.EmptyLayers(), true);
  p_matrix_.Init(data_config_);
  ASSERT_EQ(p_matrix_.EmptyLayers(), false);
}

TEST_F(PerceptronTest, GraphType) {
  ASSERT_EQ(p_graph_.EmptyLayers(), true);
  p_graph_.Init(data_config_);
  ASSERT_EQ(p_graph_.EmptyLayers(), false);
}

TEST_F(PerceptronTest, Train) {
  perceptron_.SwitchPerceptron(0);
  ASSERT_EQ(perceptron_.EmptyLayers(), true);
  perceptron_.LoadLayers(data_config_);
  perceptron_.SetEpoch(5);
  perceptron_.StartTraining(1, 0.7, "../../Tests/TestFiles/file1.csv", "");
  //    perceptron_.StartTesting("../../Tests/TestFiles/file1.csv", 0.7, 0, 1,
  //    true);
  ASSERT_EQ(perceptron_.EmptyLayers(), false);
}

TEST_F(PerceptronTest, SaveWeightMatrixTypeToLoadGraphType) {
  perceptron_.SwitchPerceptron(0);
  ASSERT_EQ(perceptron_.EmptyLayers(), true);
  perceptron_.LoadLayers(data_config_);
  perceptron_.SetEpoch(5);
  perceptron_.StartTraining(1, 0.7, "../../Tests/TestFiles/file1.csv", "");
  ASSERT_EQ(perceptron_.EmptyLayers(), false);
  perceptron_.SaveWeight("../../Tests/TestFiles/Weight.txt");
  perceptron_.SwitchPerceptron(1);
  ASSERT_EQ(perceptron_.EmptyLayers(), true);
  perceptron_.LoadWeight("../../Tests/TestFiles/Weight.txt");
  ASSERT_EQ(perceptron_.EmptyLayers(), false);
}

TEST_F(PerceptronTest, SaveWeightGraphTypeToLoadMatrixType) {
  perceptron_.SwitchPerceptron(1);
  ASSERT_EQ(perceptron_.EmptyLayers(), true);
  perceptron_.LoadLayers(data_config_);
  perceptron_.SetEpoch(5);
  perceptron_.StartTraining(1, 0.7, "../../Tests/TestFiles/file1.csv", "");
  ASSERT_EQ(perceptron_.EmptyLayers(), false);
  perceptron_.SaveWeight("../../Tests/TestFiles/Weight.txt");
  perceptron_.SwitchPerceptron(0);
  ASSERT_EQ(perceptron_.EmptyLayers(), true);
  perceptron_.LoadWeight("../../Tests/TestFiles/Weight.txt");
  ASSERT_EQ(perceptron_.EmptyLayers(), false);
}