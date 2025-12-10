#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// 局内维修
double fieldRepair(double currentMaxDurability, double currentDurability, double initialMax) {
    // ① 耐久损失
    double durabilityLoss = currentMaxDurability - currentDurability;

    // ② 损失比例
    double lossRatio = durabilityLoss / currentMaxDurability;

    // ③ 旧化系数
    double agingCoefficient = log10(currentMaxDurability / initialMax);

    // ④ 折减系数 (维修损耗固定为0.07)
    double reductionCoefficient = 0.07 - agingCoefficient;

    // ⑤ 维修后上限
    double repairedMax = currentMaxDurability - currentMaxDurability * lossRatio * reductionCoefficient;

    // ⑥ 四舍五入到小数点后1位
    return round(repairedMax * 10) / 10.0;
}

// 局外维修
int baseRepair(double currentMaxDurability, double currentDurability, double initialMax, double kitEfficiency) {
    // 局外维修第一步：向下取整当前耐久上限
    int intCurrentMax = (int)currentMaxDurability;

    // ① 耐久损失
    double durabilityLoss = intCurrentMax - currentDurability;

    // ② 损失比例
    double lossRatio = durabilityLoss / intCurrentMax;

    // ③ 旧化系数
    double agingCoefficient = log10((double)intCurrentMax / initialMax);

    // ④ 折减系数 (使用维修包效率)
    double reductionCoefficient = kitEfficiency - agingCoefficient;

    // ⑤ 维修后上限
    double repairedMax = intCurrentMax - intCurrentMax * lossRatio * reductionCoefficient;

    // ⑥ 向下取整，最小为6
    int result = (int)repairedMax;
    return (result > 6) ? result : 6;
}

int main() {
    // DICH-1头盔参数
    const double INITIAL_MAX = 40.0;
    const int MIN_SELLABLE = 28;
    const double REPAIR_LOSS = 0.07;

    double currentMax, currentDura;

    cout << "=== DICH-1头盔维修计算 ===" << endl;
    cout << "初始耐久上限: " << INITIAL_MAX << endl;
    cout << "最低可出售耐久: " << MIN_SELLABLE << endl;
    cout << "固定维修损耗: " << REPAIR_LOSS << endl << endl;

    // 输入当前耐久上限和当前耐久
    cout << "请输入当前耐久上限: ";
    cin >> currentMax;
    cout << "请输入当前耐久值: ";
    cin >> currentDura;

    // 验证输入
    if (currentDura > currentMax) {
        cout << "错误：当前耐久值不能超过耐久上限！" << endl;
        return 1;
    }

    if (currentDura < 0 || currentMax <= 0) {
        cout << "错误：耐久值必须为正数！" << endl;
        return 1;
    }

    cout << fixed << setprecision(1);

    // 局内维修
    double fieldResult = fieldRepair(currentMax, currentDura, INITIAL_MAX);
    cout << "\n--- 局内维修结果 ---" << endl;
    cout << "维修后耐久上限: " << fieldResult << endl;
    cout << "耐久减少: " << (currentMax - fieldResult) << endl;

    // 局外维修
    cout << "\n--- 局外维修结果 ---" << endl;

    // 使用不同维修包计算
    string kitNames[] = { "资质维修包", "标准维修包", "精密维修包", "高级维修包" };
    double kitEfficiencies[] = { 0.03, 0.05, 0.08, 0.56 };

    for (int i = 0; i < 4; i++) {
        int baseResult = baseRepair(currentMax, currentDura, INITIAL_MAX, kitEfficiencies[i]);
        cout << kitNames[i] << " (效率" << kitEfficiencies[i]
            << "): 维修后耐久上限 = " << baseResult;

        // 检查是否可出售
        if (baseResult >= MIN_SELLABLE) {
            cout << "可出售";
        }
        else {
            cout << "不可出售 (还需" << (MIN_SELLABLE - baseResult) << "点耐久)";
        }
        cout << endl;
    }

    // 计算耐久损失
    cout << "\n--- 当前状态 ---" << endl;
    double durabilityLoss = currentMax - currentDura;
    double lossPercentage = (durabilityLoss / currentMax) * 100;  // 修复：声明并计算 lossPercentage
    cout << "耐久损失: " << durabilityLoss << " (" << lossPercentage << "%)" << endl;

    // 旧化系数计算
    double agingCoefficient = log10(currentMax / INITIAL_MAX);
    cout << "旧化系数: " << agingCoefficient << endl;

    return 0;
}