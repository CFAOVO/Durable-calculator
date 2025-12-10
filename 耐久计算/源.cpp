#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// 头盔数据结构
struct HelmetData {
    string name;
    double initialMax;
    int minSellable;
    double repairLoss;
    double kitEfficiencies[4];
    string kitNames[4];
};

// 局内维修
double fieldRepair(double currentMaxDurability, double currentDurability, double initialMax, double repairLoss) {
    // ① 耐久损失
    double durabilityLoss = currentMaxDurability - currentDurability;

    // ② 损失比例
    double lossRatio = durabilityLoss / currentMaxDurability;

    // ③ 旧化系数
    double agingCoefficient = log10(currentMaxDurability / initialMax);

    // ④ 折减系数 (使用头盔固有的维修损耗)
    double reductionCoefficient = repairLoss - agingCoefficient;

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
    // 定义所有头盔数据
    HelmetData helmets[] = {
        {
            "DICH-1头盔",
            40.0,  // 初始上限
            28,    // 最低可出售耐久
            0.16,  // 维修损耗
            {0.05, 0.07, 0.45, 0.86}, // 维修包效率：自制、标准、精密、高级
            {"自制维修包", "标准维修包", "精密维修包", "高级维修包"}
        },
        {
            "GN重型夜视头盔",
            50.0,  // 初始上限
            35,    // 最低可出售耐久
            0.07,  // 维修损耗
            {0.05, 0.08, 0.64, 1.19}, // 维修包效率：自制、标准、精密、高级
            {"自制维修包", "标准维修包", "精密维修包", "高级维修包"}
        },
        {
            "GN重型头盔",
            50.0,  // 初始上限
            35,    // 最低可出售耐久
            0.07,  // 维修损耗
            {0.05, 0.08, 0.64, 1.19}, // 维修包效率：自制、标准、精密、高级
            {"自制维修包", "标准维修包", "精密维修包", "高级维修包"}
        },
        {
            "H07战术头盔",
            45.0,  // 初始上限
            31,    // 最低可出售耐久
            0.16,  // 维修损耗
            {0.06, 0.09, 0.79, 1.51}, // 维修包效率：自制、标准、精密、高级
            {"自制维修包", "标准维修包", "精密维修包", "高级维修包"}
        },
        {
            "Mask-1铁壁头盔",
            75.0,  // 初始上限
            52,    // 最低可出售耐久
            0.07,  // 维修损耗
            {0.11, 0.17, 1.42, 2.68}, // 维修包效率：自制、标准、精密、高级
            {"自制维修包", "标准维修包", "精密维修包", "高级维修包"}
        }
    };

    int helmetCount = sizeof(helmets) / sizeof(helmets[0]);

    cout << "=== 头盔维修计算器 ===" << endl;
    cout << "请选择要计算的头盔：" << endl;

    // 显示头盔列表
    for (int i = 0; i < helmetCount; i++) {
        cout << i + 1 << ". " << helmets[i].name << endl;
    }

    int choice;
    cout << "请输入选择编号 (1-" << helmetCount << "): ";
    cin >> choice;

    // 验证选择
    if (choice < 1 || choice > helmetCount) {
        cout << "错误：选择编号无效！" << endl;
        return 1;
    }

    // 获取选中的头盔数据
    HelmetData selectedHelmet = helmets[choice - 1];

    double currentMax, currentDura;

    cout << "\n=== " << selectedHelmet.name << " 维修计算 ===" << endl;
    cout << "初始耐久上限: " << selectedHelmet.initialMax << endl;
    cout << "最低可出售耐久: " << selectedHelmet.minSellable << endl;
    cout << "固定维修损耗: " << selectedHelmet.repairLoss << endl << endl;

    // 输入当前耐久上限和当前耐久
    cout << "请输入当前耐久上限: ";
    cin >> currentMax;
    cout << "请输入当前耐久值: ";
    cin >> currentDura;

    // 验证输入
    if (currentDura > currentMax) {
        cout << "错误：当前耐久值不能超过耐久上限" << endl;
        return 1;
    }

    if (currentDura < 0 || currentMax <= 0) {
        cout << "错误：耐久值必须为正数" << endl;
        return 1;
    }

    cout << fixed << setprecision(1);

    // 局内维修
    double fieldResult = fieldRepair(currentMax, currentDura, selectedHelmet.initialMax, selectedHelmet.repairLoss);
    cout << "\n--- 局内维修结果 ---" << endl;
    cout << "维修后耐久上限: " << fieldResult << endl;
    cout << "耐久减少: " << (currentMax - fieldResult) << endl;

    // 局外维修
    cout << "\n--- 局外维修结果 ---" << endl;

    // 使用不同维修包计算
    for (int i = 0; i < 4; i++) {
        int baseResult = baseRepair(currentMax, currentDura, selectedHelmet.initialMax, selectedHelmet.kitEfficiencies[i]);
        cout << selectedHelmet.kitNames[i] << " (效率" << selectedHelmet.kitEfficiencies[i]
            << "): 维修后耐久上限 = " << baseResult;

        // 检查是否可出售
        if (baseResult >= selectedHelmet.minSellable) {
            cout << "  可出售";
        }
        else {
            cout << "  不可出售 (还需" << (selectedHelmet.minSellable - baseResult) << "点耐久)";
        }
        cout << endl;
    }

    // 计算耐久损失
    cout << "\n--- 当前状态 ---" << endl;
    double durabilityLoss = currentMax - currentDura;
    double lossPercentage = (durabilityLoss / currentMax) * 100;
    cout << "耐久损失: " << durabilityLoss << " (" << lossPercentage << "%)" << endl;

    // 旧化系数计算
    double agingCoefficient = log10(currentMax / selectedHelmet.initialMax);
    cout << "旧化系数: " << agingCoefficient << endl;

    // 计算最大理论耐久上限
    cout << "\n--- 理论分析 ---" << endl;
    if (agingCoefficient > 0) {
        double maxTheoretical = selectedHelmet.initialMax * pow(10, selectedHelmet.repairLoss);
        cout << "最大理论耐久上限: " << maxTheoretical << endl;
        cout << "当前耐久上限/最大理论: " << (currentMax / maxTheoretical * 100) << "%" << endl;
    }

    return 0;
}