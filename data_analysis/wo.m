clear;close all;clc;
load("wo_pd.mat")
figure('units','normalized','position',[0.1,0.1,0.7,1.5])
subplot(2,1,1)
plot(wo_pd.time, wo_pd.pitch, wo_pd.time, wo_pd.filter_pitch);
xlabel("time")
ylabel("attitude")
title('时域姿态图\_模拟帕金森\_整体')
legend('俯仰角', '卡尔曼滤波后的俯仰角');

subplot(2,1,2)
plot(wo_pd.time, wo_pd.pitch, wo_pd.time, wo_pd.filter_pitch);
xlabel("time")
ylabel("attitude")
xlim([3.5 8])
title('时域姿态图\_模拟帕金森\_局部放大')
legend('俯仰角', '卡尔曼滤波后的俯仰角');
% load("wo_upd.mat")
% plot(wo_upd.time, wo_upd.pitch, wo_upd.time, wo_upd.filter_pitch, wo_upd.time, wo_upd.pid);
% legend('pitch', 'filter', 'pid\_out');