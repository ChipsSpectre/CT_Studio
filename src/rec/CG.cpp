#include "CG.h"

Eigen::VectorXf CG::conjugateGradient(int steps, DataContainer& A, Eigen::VectorXf b)
{
    // we need A^T*b and not b
    b = A.backwardProj(b);

    /*
     * This is simply a copy of the algorithm mentioned on the slides
     */
    // init the vector to zero
    Eigen::VectorXf x = Eigen::VectorXf::Zero(A.getTotalVoxelCount());

    Eigen::VectorXf Ax = A.mult(x);
    Eigen::VectorXf r = b - Ax;
    Eigen::VectorXf d = r;

    float rtr = r.dot(r);

    for(int i = 0; i < steps; ++i){
        const Eigen::VectorXf q = A.mult(d);
        const float alpha = rtr/d.dot(q);
        x += alpha * d;
        r -= alpha * q;

        const float oldRtr = rtr;
        rtr = r.dot(r);
        d = r + (rtr/oldRtr * d);
    }

    return x;
}
