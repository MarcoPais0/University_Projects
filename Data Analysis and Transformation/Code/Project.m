% Ao longo do projeto fomos fazendo pequenas alterações nas funções já
% existentes para a obtenção de dados e gráficos apresentados no 
% relatório, por isso algumas das funções poderão estar comentadas.
%% Importar dados
clear;
exp42 = load('acc_exp42_user21.txt');
exp43 = load('acc_exp43_user21.txt');
exp44 = load('acc_exp44_user22.txt');
exp45 = load('acc_exp45_user22.txt');
exp46 = load('acc_exp46_user23.txt');
exp47 = load('acc_exp47_user23.txt');
exp48 = load('acc_exp48_user24.txt');
exp49 = load('acc_exp49_user24.txt');
labels = load('labels.txt');
file = fopen('activity_labels.txt');
activity_labels = textscan(file, '%s %s');
fclose(file);
activity_labels = cell(activity_labels(1,2));

%% Gráfico do Sinal
plotSignal(exp42, 42, labels, activity_labels);

%% DFT do Sinal
activity_number = 3;
exp_number = 44;
exp = exp44;
signalDFT = plotDFTs(labels, activity_number, exp, exp_number, activity_labels{1}{activity_number});

%% Calcular Passos Estáticos
activity_number = 3;
exp_number = 42;
exp = exp42;
[media_sec_x, media_sec_y, media_sec_z] = calculateSteps(exp, exp_number, activity_number, labels)

%% Diferenciar Atividades
all_exp = cell(8,1);
all_exp{1,1} = [exp42(:,1) exp42(:,2) exp42(:,3)];
all_exp{2,1} = [exp43(:,1) exp43(:,2) exp43(:,3)];
all_exp{3,1} = [exp44(:,1) exp44(:,2) exp44(:,3)];
all_exp{4,1} = [exp45(:,1) exp45(:,2) exp45(:,3)];
all_exp{5,1} = [exp46(:,1) exp46(:,2) exp46(:,3)];
all_exp{6,1} = [exp47(:,1) exp47(:,2) exp47(:,3)];
all_exp{7,1} = [exp48(:,1) exp48(:,2) exp48(:,3)];
all_exp{8,1} = [exp49(:,1) exp49(:,2) exp49(:,3)];
figure(1);
for i=1:8
    difActivities(all_exp{i}, 41 + i, labels);
    hold on
end

%% STFT do Sinal
calculateSTFT(exp42(:,3));
