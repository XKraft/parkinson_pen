#include "kalman.h"

void kaiman_filter_init(kalman_filter_t* filter, float R_cov, float Q_cov, float P_last)
{
    filter->K = 0;
    filter->P_last = P_last;
    filter->P_now = 0;
    filter->Q_cov = Q_cov;
    filter->R_cov = R_cov;
    filter->x_last = 0;
    filter->x_now = 0;
}

//卡尔曼滤波参数更新
void kalman_filter_run(kalman_filter_t* filter, float data)
{
    filter->x_last = filter->x_now;
    filter->P_now = filter->P_last + filter->Q_cov;
    filter->K = filter->P_now / (filter->P_now + filter->R_cov);
    filter->x_now = filter->x_last + filter->K * (data - filter->x_last);
    filter->P_last = (1 - filter->K) * filter->P_now;
}