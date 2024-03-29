clear;close all;clc;
load("neng_pd.mat")

% % 初始化卡尔曼滤波器参数
% initial_state_estimate = 0; % 初始状态估计
% initial_estimate_error = 0.02; % 初始状态估计的误差方差
% process_noise_stddev = 0.02; % 状态转移噪声的标准差
% measurement_noise_stddev = 0.09;
% 
% % 初始化滤波器状态和误差方差
% state_estimate = initial_state_estimate;
% estimate_error = initial_estimate_error;
% 
% % 存储估计的状态
% filtered_state_estimates = zeros(1, length(neng_pd.pitch));
% 
% % 卡尔曼滤波
% for t = 1:length(neng_pd.pitch)
%     % 预测步骤（状态估计的预测）
%     predicted_state_estimate = state_estimate;
%     predicted_estimate_error = estimate_error + process_noise_stddev^2;
%     
%     % 更新步骤（根据观测值来更新状态估计）
%     kalman_gain = predicted_estimate_error / (predicted_estimate_error + measurement_noise_stddev^2);
%     state_estimate = predicted_state_estimate + kalman_gain * (neng_pd.pitch(t) - predicted_state_estimate);
%     estimate_error = (1 - kalman_gain) * predicted_estimate_error;
%     
%     % 存储估计的状态
%     filtered_state_estimates(t) = state_estimate;
% end

k_p = 20;
k_i = -20;
pid_last_p = 0;
pid_p = 0;
pid_i = 0;
pid_out = zeros(1 , length(neng_pd.pitch));

for i = 1 : length(neng_pd.pitch)
    pid_last_p = pid_p;
    pid_p = neng_pd.pitch(i) - neng_pd.filter_pitch(i);
    pid_i = pid_p - pid_last_p;
    pid_out(i) = k_p * pid_p + k_i * pid_i;
end

plot(neng_pd.time, neng_pd.pitch, neng_pd.time, neng_pd.filter_pitch, neng_pd.time, pid_out, neng_pd.time, neng_pd.pid);
legend('pitch', 'filter', 'pid_out', 'pid');