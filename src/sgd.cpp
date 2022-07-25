#include "algorithms.h"
#include "matrix.h"

void SGD::Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) 
{
    assert(X_train.GetRows() == y_train.GetRows());
    // лучше перемешивание делать в вызывающей прогармме, потому что иначе нужно создавать копию датасета
    std::mt19937 rdnX(123);
    std::mt19937 rdnY(123);
    Linalg::Matrix<double> X_copy = X_train;
    Linalg::Matrix<double> y_copy = y_train;

    std::shuffle(X_copy.IterRow(0), X_copy.IterRow(X_copy.GetRows()), rdnX);
    std::shuffle(y_copy.IterRow(0), y_copy.IterRow(X_copy.GetRows()), rdnY);
    
    w = WeightsMinErr(X_copy, y_copy); // w = (0,...,0) ?
    // w.Assign(X_train.GetCols(), 1); // w = (0,...,0) ?

    double originalErr = MAE(X_copy * w, y_copy);
    double Q = MSE(X_copy * w, y_copy);
    std::cout << "Q : " << Q << std::endl;
    double prevQ = 0;

    Linalg::Matrix<double> f;
    Linalg::Matrix<double> err;
    Linalg::Matrix<double> grad;
    double prevErr = 0;
    int N = 10000;
    int B = 500;
    double fr = 0.005;
    // при B = 1 бодро спускается к минимуму
    size_t i = 0;

    auto itX = X_copy.cIterRow(0);
    auto itY = y_copy.cIterRow(0);
    for (; i < N; ++i)
    {
        // - реализовать случайный выбор B объектов из выбоки
        for (size_t j = B; j < X_copy.GetRows(); j += B)
        {
            Linalg::Matrix<double> X_batch(itX + (j - B), itX + j);
            Linalg::Matrix<double> y_batch(itY + (j - B), itY + j);
            f = X_batch * w;
            err = f - y_batch;
            // / X_batch.GetRows() это действие принесло 2.14 на numeric_scaled (было 2.36)
            grad = 2 * (X_batch.T() * err) / X_batch.GetRows();
            w -= lr * grad;
            Q = fr * err.Sqr().Mean() + (1 - fr) * Q;
        }
        // для MSE "< 10000"
        if (std::abs(prevQ - Q) < 10000000) 
        {
            std::cout << "Q = " << Q  << std::endl;
            std::cout << prevQ << " " << Q << std::endl;
            std::cout << (long long)prevQ << " " << (long long)Q << std::endl;
            break;
        }

        std::cout << "Q = " << Q  << std::endl;
        // usleep(1e4);
        prevQ = Q;
        // std::cout << "MAE = " << MAE(X_copy * w, y_copy) << std::endl;
        // if (i % 100 == 0)
        // {
        //     double curErr = AE(X_copy * w, y_copy,);
        //     if (prevErr == curErr)
        //     {
        //         break;
        //     }
        //     prevErr = curErr;
        //     std::cout << "AE = " << curErr << std::endl;
        // }
        
        // std::cout << "Q = " << 
        // if (i % 100 == 0)
        // {
        //     if (prevQ == Q)
        //     {
        //         break;
        //     }
        //     prevQ = Q;
        // }
    }
    w.Show("\t\tweights");
    std::cout << "MAE = " << MAE(X_copy * w, y_copy) << std::endl;
    std::cout << "original MAE = " << originalErr << std::endl; 
    std::cout << "Itarations = " << i << std::endl;
}

Linalg::Matrix<double> SGD::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    return X_test * w;
}