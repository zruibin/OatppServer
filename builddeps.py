#! /usr/bin/env python3
# -*- coding: utf-8 -*- 
#
# builddeps.py
#
# Created by Ruibin.Chow on 2022/01/26.
# Copyright (c) 2022年 Ruibin.Chow All rights reserved.
# 

"""

"""

import os, re, json, sys, platform
import subprocess, shutil
import datetime

homeDir = ""
releaseDir = ""
depsDir = ""
depsDirName = "deps"
releaseDirName = "release"
slash = "/"
if(platform.system()=='Windows'):
    slash = "\\"

logList = []

def logRecord():
    with open('builddeps.log', 'w') as fileHandle:
        for logStr in logList:
            fileHandle.write(str(logStr))

def log(string="", newline=True):
    if newline:
        logList.append(str(string) + "\n")
        print(string, end="\n")
    else:
        logList.append(str(string))
        print(string, end="")
    pass

def operator(cmdString, newline=True):
    log(cmdString)
    output = os.popen(cmdString)
    for line in output.readlines():
        log(line, newline)

def operatorCMD(parameterList, newline=True):
    cmdString = " ".join(parameterList)
    operator(cmdString, newline)
    pass


def buildDeps(dirStr, gitUrl, cloneList, cmdList, genBuilding=True, preCmdList=[]):
    if len(dirStr) == 0 and len(gitUrl) == 0 and len(cloneList) == 0 and len(cmakeList) == 0:
        log("Building Deps Was Error!")
        return
    log("-"*80)
    log("Start Building Deps: " + dirStr)
    os.chdir(depsDir)
    operatorCMD(cloneList)
    os.chdir(dirStr)
    if genBuilding:
        buildDir = "build"
        if os.path.exists(buildDir):
            shutil.rmtree(buildDir)
        os.makedirs(buildDir)
        os.chdir(buildDir)
    log("当前编译路径：" + os.getcwd())
    if len(preCmdList) > 0:
        operatorCMD(preCmdList, False)    
    operatorCMD(cmdList, False)
    operator("make", False)
    operator("make install", False)
    pass

def buildDepsByList(buildDepsList, genBuilding=True, preCmdList=[]):
    if len(buildDepsList) < 4:
        log("Building Deps List Was Error!")
        return
    buildDeps(buildDepsList[0], buildDepsList[1], buildDepsList[2], buildDepsList[3], genBuilding, preCmdList)
    pass

def genDepsCmakeList():
    log("-"*80)
    log("Generate Deps CmakeList in Path: " + homeDir)
    os.chdir(homeDir)
    depsCamke = "deps.cmake"
    depsContent = """
message("This is deps.cmake")

set(deps_list SQLiteCpp sqlite3 pthread dl)

set(DEPS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/release/include")
set(DEPS_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/release/lib")

message("Deps Include Directory: ${DEPS_INCLUDE_DIR}")
message("Deps Lib Directory: ${DEPS_LIB_DIR}")

include_directories("${DEPS_INCLUDE_DIR}")
link_directories("${DEPS_LIB_DIR}")
"""
    log("Deps CmakeList content: " + depsContent)
    with open(depsCamke, "w") as fileHandler:
        fileHandler.write(depsContent)

    pass


if __name__ == '__main__':
    begin = datetime.datetime.now()
    log("更新时间：" + str(begin))

    if not os.path.exists(depsDirName):
        os.makedirs(depsDirName)
    if not os.path.exists(releaseDirName):
        os.makedirs(releaseDirName)
    
    homeDir = sys.path[0]
    log("Home Directory: " + homeDir)
    depsDir = homeDir + slash + depsDirName
    log("Deps Directory: " + depsDir)
    releaseDir = homeDir + slash + releaseDirName
    log("Install Directory: " + releaseDir)

    abseilList = [
        "abseil",
        "https://github.com/abseil/abseil-cpp",
        ["git", "clone", "-b", "20211102.0", "--depth=1", "https://github.com/abseil/abseil-cpp", "abseil"],
        ["cmake", "-D CMAKE_BUILD_TYPE=RELEASE", 
            "-DABSL_BUILD_TESTING=ON -DABSL_USE_GOOGLETEST_HEAD=OFF -DCMAKE_CXX_STANDARD=11", 
            "-D", "CMAKE_INSTALL_PREFIX="+releaseDir, ".."]
    ]
    buildDepsByList(abseilList)

    hiredisList = [
        "hiredis",
        "https://github.com/redis/hiredis",
        ["git", "clone", "-b", "v1.0.2", "--depth=1", "https://github.com/redis/hiredis", "hiredis"],
        ["cmake", "-D CMAKE_BUILD_TYPE=RELEASE", 
            "-D", "CMAKE_INSTALL_PREFIX="+releaseDir, ".."]
    ]
    buildDepsByList(hiredisList)

    sqliteCppList = [
        "SQLiteCpp",
        "https://github.com/SRombauts/SQLiteCpp",
        ["git", "clone", "-b", "3.1.1", "--depth=1", "https://github.com/SRombauts/SQLiteCpp", "SQLiteCpp"],
        ["cmake", "-D CMAKE_BUILD_TYPE=RELEASE", 
            "-D", "CMAKE_INSTALL_PREFIX="+releaseDir, ".."]
    ]
    buildDepsByList(sqliteCppList)
    
    # libconfiniList = [
    #     "libconfini",
    #     "https://github.com/madmurphy/libconfini",
    #     ["git", "clone", "-b", "1.16.3", "--depth=1", "https://github.com/madmurphy/libconfini", "libconfini"],
    #     ["./configure", "--prefix="+releaseDir]
    # ]
    # buildDepsByList(libconfiniList, genBuilding=False, preCmdList=["./bootstrap"])

    genDepsCmakeList()
    end = datetime.datetime.now()
    log(('花费时间: %.3f 秒' % (end - begin).seconds))
    logRecord()
    pass





