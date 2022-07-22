#include "matrix.h"
#include "algorithms.h"

int main(int argc, char *argv[])
{

    std::ifstream file("../" + std::string(argv[1]));
    if (!file)
    {
        return 0;
    }
    int n, d;
    file >> n >> d;
    Linalg::Matrix<double> X_train(n, d);
    Linalg::Matrix<double> y_train(n, 1);
    size_t i = 0;
    while (!file.eof())
    {
        for (size_t j = 0; j < d; ++j)
        {
            file >> X_train[i][j];
        }
        file >> y_train[i][0];
        ++i;
    }
    

    SGD model;
    model.Fit(X_train, y_train);


    Linalg::Matrix<double> y_pred = model.Predict({{{16, 79, 0, 1}}});
    y_pred.Show();
    





    // std::function<double(double)> df = [](double x) -> double
    // {
    //     return 4 * x * x * x - 12 * x * x + 4;
    // };
    // double x_min = FindExtremum(df, std::minus<double>());
    // std::cout << "x_min: " << x_min << std::endl;
}