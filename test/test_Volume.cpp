#include "catch.hpp"

#include <iostream>

#include <Eigen/Eigen>

#include "Volume.h"

using M=Eigen::VectorXf;

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test volume creation")
{
    int n = 2;
    M m(n*n*n);
    for(int i = 0; i<n; ++i)
    {
        for(int j = 0; j<n; ++j)
        {
            for(int k = 0; k<n; ++k)
            {
                m(i*n*n+j*n+k) = (i+j+k)/8.0;
            }
        }
    }

    Eigen::Vector3f lowerLeft(0, 0, 0);
    Eigen::Vector3f upperRight(2, 2, 2);
    Eigen::Vector3f dimSpacing(1, 1, 1);

    Volume vol(lowerLeft, upperRight, dimSpacing, std::move(m));

    SECTION("trilinear interpolation test in volume size")
    {
        // go through one non-int line of the volume
        // and check to known values
        Eigen::VectorXf result(11);
        for(int i = 0; i<11; ++i)
        {
            // after ten steps 3/8 is reached
            // this has to happen in equal intervals
            result(i) = 3.0/80.0*i;
        }

        for(int i = 0; i<=10; ++i)
        {
            Eigen::Vector3f pos(0.1*i, 0.1*i, 0.1*i);
            float f = vol.getVoxelLinear(pos);

            REQUIRE(f == Approx(result(i)));
        }
    }

    SECTION("trilinear interpolation test in physical size")
    {
        Eigen::Vector3f lowerLeft2(0, 0, 0);
        Eigen::Vector3f upperRight2(2, 2, 2);
        Eigen::Vector3f dimSpacing2(1, 1, 1);
        M m2(n*n*n);
        for(int i = 0; i<n; ++i)
        {
            for(int j = 0; j<n; ++j)
            {
                for(int k = 0; k<n; ++k)
                {
                    m2(i*n*n+j*n+k) = (i+j+k)/8.0;
                }
            }
        }
        Eigen::VectorXf result(10);
        for(int i = 0; i<10; ++i)
        {
            // after ten steps 3/8 is reached
            // this has to happen in equal intervals
            result(i) = 3.0/80.0*i;
        }

        Volume vol2(lowerLeft2, upperRight2, dimSpacing2, std::move(m2));
        for(int i = 0; i<20; ++i)
        {
            // divide position by ten (because volume is more little, too
            Eigen::Vector3f pos(-1+0.1*i, -1+0.1*i, -1+0.1*i);
            float f = vol2.getVoxelLinearPhysical(pos);
            std::cout << f << std::endl;

            //REQUIRE(f == Approx(result(i)));

        }

    }

    SECTION("coordinate to index test")
    {
        // handle special case of zero position
        Eigen::Vector3i position(0,0,0);
        int x = vol.coordinateToIndex(position);
        REQUIRE(x == 0);
    }
}
