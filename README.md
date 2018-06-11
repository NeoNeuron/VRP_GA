# VRP_GA

Vehicle routing problem (VRP) with Genetic Algorithm (GA)

This repository provides program and module to solve vehicle routing problem with genetic alorithm.

## Compile
Install gcc/g++ compiler using MinGW-win64 for Windows user, or using `apt-get` to install g++ for Liunx user.

- For Windows user:
```
g++ -O2 ga.cpp get-config.cpp main.cpp -o run.exe
```
- For Linux user:
```
g++ -O2 ga.cpp get-config.cpp main.cpp -o run.out
```
## Excecution
- For Windows user:
```
run.exe
```
- For Linux user:
```
./run.out
```
### Result visualization:
*Data visualization requires `Python2.7` or `Python3.6`, with `numpy` and `matplotlib` package installed*

Program can output optimal routing strategy to `optimal_strategy.csv`, its corresponding convergent curve of total path distance to `optimal_least_path.csv`. After that, use the following commond:
```
python plt_result.py
```
to visualize the optimal result;

## Parameter tuning
Please find discription and comments in `config.ini`;

## 编译
安装gcc/g++编译器。Windows用户可使用MinGW-win64软件包管理器安装，Linux用户直接通过`apt-get`命令安装g++即可。

切换至仓库根目录下。
对于Windows用户，
```
g++ -O2 ga.cpp get-config.cpp main.cpp -o run.exe
```
对于Linux用户，
```
g++ -O2 ga.cpp get-config.cpp main.cpp -o run.out
```

## 运行程序
双击`run`或在命令指示符（Terminal）运行。
对于Windows用户，
```
run.exe
```
对于Linux用户，
```
./run.out
```

### 数据可视化
*数据可视化需要`Python2.7`或`Python3.6`环境，以及`numpy`和`matplotlib`包的支持*

程序将生成最优车辆路径方案，输出到文件`optimal_strategy.csv`，以及其对应的最短路径随程序迭代的收敛关系，输出到文件`optimal_least_path.csv`。

生成结果后，运行python脚本`plt_result.py`,进行数据结果可视化。
```
python plt_result.py
```
结果输出到图片`optimal_strategy.png`和`optimal_least_path.png`。

## 配置文件说明：

### 遗传算法参数
- `PopSize` 每一代种群中包含独立个体数目
- `CrossoverProbability` 交叉操作发生概率
- `CrossoverLength` 每次两点交叉操作中，交换染色体片段中所包含的基因数目
- `MutateProbability` 变异操作发生概率
- `MutateNumber` 每次变异操作中，随机交换基因对的数目
- `MaxCapacity` 每辆货车最大载重量
- `Seed` 初始化种群时使用的随机数种子

### 种群繁衍代数
- `PreScanGenerationNumber` 预搜索种群繁衍代数
- `DeepSearchGenerationNumber` 深度搜索种群繁衍代数

### [车辆总路径下边界参数]
#### 预搜索下边界参数
- `PreScanInitialBound` 初始下边界
- `PreScanBoundStep` 每次边界下降步长
- `PreScanNumStep` 边界下降次数

#### 深度搜索下边界参数
- `DeepSearchNumber` 深度搜索候选点数目
- `DeepSearchInitialBound` 初始下边界
- `DeepSearchBoundStep` 每次边界下降步长
- `DeepSearchNumStep` 边界下降次数

- `OutputFlag = true` 输出到文件标志
