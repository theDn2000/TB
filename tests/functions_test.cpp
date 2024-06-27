#include "gtest/gtest.h"
#include "../functions.h"


using namespace std;

// Trilateration test
TEST(TrilaterationTest, TrilaterationTest)
{
    vector<float> P1 = {0, 0};
    vector<float> P2 = {-1, 0};
    vector<float> P3 = {0, -1};
    float D1 = 1;
    float D2 = 1;
    float D3 = 1;
    vector<float> result = trilateration(P1, P2, P3, D1, D2, D3);
    vector<float> expected = {-0.5, -0.5};
    for (int i = 0; i < 2; i++)
    {
        cout << "Result: " << result[i] << " Expected: " << expected[i] << endl;
        EXPECT_NEAR(result[i], expected[i], 0.0001);
    }
}
