#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import time
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as st
import librosa  # https://librosa.org/
import librosa.display
import librosa.beat
import sounddevice as sd  # https://anaconda.org/conda-forge/python-sounddevice
import warnings
from scipy.spatial import distance


def readFeatures(fileName):  # 2.1.1
    top100 = np.genfromtxt(fileName, delimiter=',')
    lines, columns = np.shape(top100)
    # print("dim ficheiro top100_features.csv original = %d x %d\n\n" % (lines, columns), top100)
    top100 = top100[1:, 1:(columns-1)]
    """lines, columns = np.shape(top100)
    print("\ndim top100 data = %d x %d\n\n" % (lines, columns), top100)
    print()"""
    return top100


def normalization(features):  # 2.1.2
    features_N = np.zeros(np.shape(features))
    columns = np.shape(features_N)[1]
    for i in range(columns):
        vmax = features[:, i].max()
        vmin = features[:, i].min()
        features_N[:, i] = (features[:, i] - vmin)/(vmax - vmin)
    # print(features_N)
    return features_N


def saveFeatures(fileName, features):  # 2.1.3
    np.savetxt(fileName, features, fmt="%lf", delimiter=',')

    # check if it's all good
    features = np.genfromtxt(fileName, delimiter=',')
    lines, columns = features.shape
    print("dim ficheiro %s = %d x %d\n\n" %
          (fileName, lines, columns), features)
    print()


def loadAudio(fileName, sr, mono):
    warnings.filterwarnings("ignore")
    y, fs = librosa.load(fileName, sr=sr, mono=mono)
    # print(y.shape)
    # print(fs)
    return y, fs


# --- Extract features
def extracFeatures():  # 2.2.2
    if not os.path.isdir(audioDir):
        print("Directory not found!")
        return

    warnings.filterwarnings("ignore")

    # List of songs to extract features
    files = os.listdir(audioDir)
    files.sort()    # sort the files alphabetically
    numFiles = len(files)
    allSongs = np.zeros((numFiles, 190))

    # Extract features of each song
    for i in range(numFiles):
        features = []

        # Song to extract
        inFile = librosa.load(audioDir + files[i], sr=22050, mono=True)[0]

        # Extract mfcc
        mfcc = librosa.feature.mfcc(inFile, n_mfcc=13)
        features.append(mfcc)

        # Extract spectral centroid
        spectral_centroid = librosa.feature.spectral_centroid(inFile)
        features.append(spectral_centroid)

        # Extract spectral bandwith
        spectral_bandwidth = librosa.feature.spectral_bandwidth(inFile)
        features.append(spectral_bandwidth)

        # Extract spectral contrast
        spectral_contrast = librosa.feature.spectral_contrast(
            inFile, n_bands=6)
        features.append(spectral_contrast)

        # Extract spectral flatness
        spectral_flatness = librosa.feature.spectral_flatness(inFile)
        features.append(spectral_flatness)

        # Extract spectral rollof
        spectral_rolloff = librosa.feature.spectral_rolloff(inFile)
        features.append(spectral_rolloff)

        # Extract F0
        F0 = librosa.yin(inFile, fmin=20, fmax=11025)
        F0[F0 == 11025] = 0
        features.append(F0)

        # Extract rms
        rms = librosa.feature.rms(inFile)
        features.append(rms)

        # Extract zero_crossing_rate
        zero_crossing_rate = librosa.feature.zero_crossing_rate(inFile)
        features.append(zero_crossing_rate)

        # Extract tempo
        tempo = librosa.beat.tempo(inFile)

        allFeatures = np.array([])
        for feature in features:
            try:
                r, _ = feature.shape
            except:
                r = feature.shape[0]
                feature = feature.reshape((1, r), order='F')
                r, _ = feature.shape

            addFeature = np.zeros((r, 7))
            for j in range(r):
                mean = np.mean(feature[j, :])
                stdDev = np.std(feature[j, :])
                skew = st.skew(feature[j, :])
                kurtosis = st.kurtosis(feature[j, :])
                median = np.median(feature[j, :])
                maxv = np.max(feature[j, :])
                minv = np.min(feature[j, :])

                addFeature[j, :] = np.array(
                    [mean, stdDev, skew, kurtosis, median, maxv, minv])

            addFeature = addFeature.flatten()
            allFeatures = np.append(allFeatures, addFeature)

        allFeatures = np.append(allFeatures, tempo)
        allSongs[i] = allFeatures

    return allSongs


def metricas_top100(filename):
    feat = np.genfromtxt(filename, delimiter=",")
    feat = feat[1:, 1:(feat.shape[1]-1)]

    euclidiana = np.zeros((900, 900))
    manhattan = np.zeros((900, 900))
    cosseno = np.zeros((900, 900))

    for i in range(900):
        for j in range(i+1, 900):
            euclidiana[i][j] = distance.euclidean(
                feat[i, :], feat[j, :])
            euclidiana[j][i] = euclidiana[i][j]
            manhattan[i][j] = distance.cityblock(
                feat[i, :], feat[j, :])
            manhattan[j][i] = manhattan[i][j]
            cosseno[i][j] = distance.cosine(feat[i, :], feat[j, :])
            cosseno[j][i] = cosseno[i][j]

    np.savetxt(euclidianaFile,
               euclidiana, fmt="%f", delimiter=",")
    np.savetxt(manhattanFile,
               manhattan, fmt="%f", delimiter=",")
    np.savetxt(cossenoFile,
               cosseno, fmt="%f", delimiter=",")


def metricas_normalizada(filename):
    feat = np.genfromtxt(filename, delimiter=",")

    for i in range(900):
        for j in range(190):
            if (np.isnan(feat[i][j])):
                feat[i][j] = 0

    euclidiana = np.zeros((900, 900))
    manhattan = np.zeros((900, 900))
    cosseno = np.zeros((900, 900))

    for i in range(900):
        for j in range(i+1, 900):
            euclidiana[i][j] = distance.euclidean(
                feat[i, :], feat[j, :])
            euclidiana[j][i] = euclidiana[i][j]
            manhattan[i][j] = distance.cityblock(
                feat[i, :], feat[j, :])
            manhattan[j][i] = manhattan[i][j]
            cosseno[i][j] = distance.cosine(feat[i, :], feat[j, :])
            cosseno[j][i] = cosseno[i][j]

    np.savetxt(euclidiana_normalizada,
               euclidiana, fmt="%f", delimiter=",")
    np.savetxt(manhattan_normalizada,
               manhattan, fmt="%f", delimiter=",")
    np.savetxt(cosseno_normalizada,
               cosseno, fmt="%f", delimiter=",")


def similarity(filename, music):
    feat = np.genfromtxt(filename, delimiter=",")
    return np.argsort(feat[music][:])[1:21]


def getMetadata(music):
    metadataRawMatrix = np.genfromtxt(
        'dataset/panda_dataset_taffc_metadata.csv', delimiter=',', dtype="str")
    metadata = metadataRawMatrix[1:, [1, 3, 9, 11]]

    metadataScores = np.zeros(900)
    metadataScores[music] = -1
    for i in range(metadata.shape[0]):
        score = 0
        if i == music:
            continue
        for j in range(metadata.shape[1]):
            # teste para artista e quadrante:
            if j < 2:
                if metadata[music, j] == metadata[i, j]:
                    score = score + 1
            else:
                # teste para MoodStrSplit e GenresStr
                # retira as "" do começo e fim e separa no "; "
                listA = metadata[music, j][1:-1].split('; ')
                listB = metadata[i, j][1:-1].split('; ')
                for e in listA:
                    for ee in listB:
                        if e == ee:
                            score = score + 1
        metadataScores[i] = score

    top20 = np.flip(np.argsort(metadataScores))[:20]

    return metadataScores, top20


if __name__ == "__main__":
    plt.close('all')

    # --- Ex2.1
    top100File = './Features/top100_features.csv'
    # top100 = readFeatures(top100File)
    # top100File_N = top100File.replace('.csv', '_normalized_data.csv')
    # top100_N = normalization(top100)
    # saveFeatures(top100File_N, top100_N)

    # --- Ex2.2
    # start = time.time()
    audioDir = 'dataset/audios/'
    # allSongs = extracFeatures()
    # allSongs_N = normalization(allSongs)
    allSongsFile_N = './Features/All_features_normalized_data.csv'
    # saveFeatures(allSongsFile_N, allSongs_N)
    # final = time.time() - start
    # print(f"\n\033[94mTIME: {final}\033[0m")

    # --- Ex3
    euclidianaFile = "./Features/euclidiana_top100.csv"
    manhattanFile = "./Features/manhattan_top100.csv"
    cossenoFile = "./Features/cosseno_top100.csv"

    euclidiana_normalizada = "./Features/euclidiana_normalizada.csv"
    manhattan_normalizada = "./Features/manhattan_normalizada.csv"
    cosseno_normalizada = "./Features/cosseno_normalizada.csv"

    files = os.listdir(audioDir)
    files.sort()
    songs = [files.index("MT0000202045.mp3"), files.index("MT0000379144.mp3"), files.index(
        "MT0000414517.mp3"), files.index("MT0000956340.mp3")]
    
    # metricas_top100(top100File)
    # metricas_normalizada(allSongsFile_N)

    with open("similarity.txt", "w") as f, open("metadataScores.txt", "w") as ff:
        for i in [euclidianaFile, manhattanFile, cossenoFile,
                  euclidiana_normalizada, manhattan_normalizada, cosseno_normalizada]:
            f.write(f"---------- {i} ----------\n")
            top20_3, top20_4 = [], []

            for song in songs:
                index = similarity(i, song)
                aux = []
                for k in range(20):
                    aux.append(files[index[k]].replace(".mp3", ""))

                f.write(f"{files[song]}\n")
                for a in aux:
                    f.write(f"{a} | ")
                f.write("\n\n")
                top20_3.append(aux)

                # --- Ex 4
                metadataScores, top20 = getMetadata(song)
                aux = []
                for top in top20:
                    aux.append(files[top].replace(".mp3", ""))

                if i == euclidianaFile:  # prevents writing the same in the file
                    ff.write("\nSEARCHING MATCHES FOR " + files[song] + "\n")
                    ff.write(metadataScores.__str__() + "\n")
                    ff.write(f"\nTOP 20 FOR {files[song]}\n")
                    for a in aux:
                        ff.write(f"{a} | ")
                    
                    ff.write(
                        "\n----------------------------------------------------------------\n")

                top20_4.append(aux)

            for k in range(4):
                precision = len(np.intersect1d(top20_3[k], top20_4[k])) / 20
                print(f"Precision of {files[songs[k]]} - {i} -> {precision}")
