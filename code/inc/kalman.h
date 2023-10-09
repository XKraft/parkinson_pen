#ifndef _KALMAN_
#define _KALMAN_

typedef struct kalman_filter_t
{
    float P_now;
    float P_last;
    float x_now;
    float x_last;
    float K;
    float R_cov;//待调参
    float Q_cov;//待调参

} kalman_filter_t;

void kaiman_filter_init(kalman_filter_t* filter, float R_cov, float Q_cov, float P_last);
void kalman_filter_run(kalman_filter_t* filter, float data);

#endif