#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

// 跨平台清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 装备类型枚举
enum EquipmentType {
    HELMET,
    ARMOR,
    UNKNOWN
};

// 装备数据结构
struct EquipmentData {
    string name;
    EquipmentType type;
    double initialMax;
    int minSellable;
    double repairLoss;
    vector<double> kitEfficiencies;
    vector<string> kitNames;
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

// 初始化头盔数据
vector<EquipmentData> initializeHelmets() {
    vector<EquipmentData> helmets;

    EquipmentData helmet;

    // DICH-1头盔
    helmet.name = "DICH-1头盔";
    helmet.type = HELMET;
    helmet.initialMax = 40.0;
    helmet.minSellable = 28;
    helmet.repairLoss = 0.16;
    helmet.kitEfficiencies = { 0.05, 0.07, 0.45, 0.86 };
    helmet.kitNames = { "自制维修包", "标准维修包", "精密维修包", "高级维修包" };
    helmets.push_back(helmet);

    // GN重型夜视头盔
    helmet.name = "GN重型夜视头盔";
    helmet.type = HELMET;
    helmet.initialMax = 50.0;
    helmet.minSellable = 35;
    helmet.repairLoss = 0.07;
    helmet.kitEfficiencies = { 0.05, 0.08, 0.64, 1.19 };
    helmet.kitNames = { "自制维修包", "标准维修包", "精密维修包", "高级维修包" };
    helmets.push_back(helmet);

    // GN重型头盔
    helmet.name = "GN重型头盔";
    helmet.type = HELMET;
    helmet.initialMax = 50.0;
    helmet.minSellable = 35;
    helmet.repairLoss = 0.07;
    helmet.kitEfficiencies = { 0.05, 0.08, 0.64, 1.19 };
    helmet.kitNames = { "自制维修包", "标准维修包", "精密维修包", "高级维修包" };
    helmets.push_back(helmet);

    // H07战术头盔
    helmet.name = "H07战术头盔";
    helmet.type = HELMET;
    helmet.initialMax = 45.0;
    helmet.minSellable = 31;
    helmet.repairLoss = 0.16;
    helmet.kitEfficiencies = { 0.06, 0.09, 0.79, 1.51 };
    helmet.kitNames = { "自制维修包", "标准维修包", "精密维修包", "高级维修包" };
    helmets.push_back(helmet);

    // Mask-1铁壁头盔
    helmet.name = "Mask-1铁壁头盔";
    helmet.type = HELMET;
    helmet.initialMax = 75.0;
    helmet.minSellable = 52;
    helmet.repairLoss = 0.07;
    helmet.kitEfficiencies = { 0.11, 0.17, 1.42, 2.68 };
    helmet.kitNames = { "自制维修包", "标准维修包", "精密维修包", "高级维修包" };
    helmets.push_back(helmet);

    return helmets;
}

// 初始化护甲数据（预留接口，目前为空）
vector<EquipmentData> initializeArmors() {
    vector<EquipmentData> armors;
    // 预留护甲数据接口，未来可以添加护甲数据
    return armors;
}

// 显示装备选择菜单
int displayEquipmentMenu(const vector<EquipmentData>& equipmentList, const string& equipmentType) {
    clearScreen();

    cout << "=== " << equipmentType << "维修计算器 ===" << endl;
    cout << "请选择要计算的" << equipmentType << "：" << endl;

    // 显示装备列表
    for (size_t i = 0; i < equipmentList.size(); i++) {
        cout << i + 1 << ". " << equipmentList[i].name << endl;
    }
    cout << "0. 返回上级菜单" << endl;

    int choice;
    cout << "\n请输入选择编号 (0-" << equipmentList.size() << "): ";
    cin >> choice;

    return choice;
}

// 执行维修计算
void performRepairCalculation(const EquipmentData& equipment) {
    double currentMax, currentDura;

    clearScreen();
    cout << "=== " << equipment.name << " 维修计算 ===" << endl;
    cout << "初始耐久上限: " << equipment.initialMax << endl;
    cout << "最低可出售耐久: " << equipment.minSellable << endl;
    cout << "固定维修损耗: " << equipment.repairLoss << endl << endl;

    // 输入当前耐久上限和当前耐久
    cout << "请输入当前耐久上限: ";
    cin >> currentMax;
    cout << "请输入当前耐久值: ";
    cin >> currentDura;

    // 验证输入
    if (currentDura > currentMax) {
        cout << "\n错误：当前耐久值不能超过耐久上限！按任意键返回..." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    if (currentDura < 0 || currentMax <= 0) {
        cout << "\n错误：耐久值必须为正数！按任意键返回..." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    // 清屏并显示计算结果
    clearScreen();

    cout << fixed << setprecision(1);

    // 显示装备信息
    cout << "=== " << equipment.name << " 维修计算结果 ===" << endl;
    cout << "初始耐久上限: " << equipment.initialMax;
    cout << " | 最低可出售耐久: " << equipment.minSellable;
    cout << " | 维修损耗: " << equipment.repairLoss << endl;
    cout << "当前耐久上限: " << currentMax;
    cout << " | 当前耐久值: " << currentDura << endl;

    // 计算耐久损失和旧化系数
    double durabilityLoss = currentMax - currentDura;
    double lossPercentage = (durabilityLoss / currentMax) * 100;
    double agingCoefficient = log10(currentMax / equipment.initialMax);

    cout << "耐久损失: " << durabilityLoss << " (" << lossPercentage << "%)";
    cout << " | 旧化系数: " << agingCoefficient << endl;
    cout << string(60, '-') << endl;

    // 局内维修
    double fieldResult = fieldRepair(currentMax, currentDura, equipment.initialMax, equipment.repairLoss);
    cout << "--- 局内维修结果 ---" << endl;
    cout << "维修后耐久上限: " << fieldResult;
    cout << " | 耐久减少: " << (currentMax - fieldResult) << endl;

    // 局外维修
    cout << "\n--- 局外维修结果 ---" << endl;

    // 使用不同维修包计算
    for (size_t i = 0; i < equipment.kitEfficiencies.size(); i++) {
        int baseResult = baseRepair(currentMax, currentDura, equipment.initialMax, equipment.kitEfficiencies[i]);
        cout << equipment.kitNames[i] << " (效率" << equipment.kitEfficiencies[i]
            << "): 维修后耐久上限 = " << baseResult;

        // 检查是否可出售
        if (baseResult >= equipment.minSellable) {
            cout << " 可出售";
        }
        else {
            cout << " 不可出售 (还需" << (equipment.minSellable - baseResult) << "点耐久)";
        }
        cout << endl;
    }

    // 理论分析
    cout << "\n--- 理论分析 ---" << endl;
    if (agingCoefficient > 0) {
        double maxTheoretical = equipment.initialMax * pow(10, equipment.repairLoss);
        cout << "最大理论耐久上限: " << maxTheoretical;
        cout << " | 当前耐久上限/最大理论: " << (currentMax / maxTheoretical * 100) << "%" << endl;
    }
    else {
        cout << "当前耐久上限未超过初始值，无需计算最大理论值" << endl;
    }

    // 判断建议
    cout << "\n--- 维修建议 ---" << endl;
    double fieldLoss = currentMax - fieldResult;
    if (fieldLoss < 0.5) {
        cout << "局内维修耐久损失很小(" << fieldLoss << ")，建议使用局内维修节省维修包。" << endl;
    }
    else {
        cout << "局内维修耐久损失较大(" << fieldLoss << ")，建议使用局外维修。" << endl;
    }

    // 显示最佳维修包选择
    bool anyUsable = false;
    for (size_t i = 0; i < equipment.kitEfficiencies.size(); i++) {
        int baseResult = baseRepair(currentMax, currentDura, equipment.initialMax, equipment.kitEfficiencies[i]);
        if (baseResult >= equipment.minSellable) {
            if (!anyUsable) {
                cout << "可使用的维修包: ";
                anyUsable = true;
            }
            else {
                cout << ", ";
            }
            cout << equipment.kitNames[i];
        }
    }

    if (!anyUsable) {
        cout << "当前没有任何维修包能使装备达到可出售状态。" << endl;
    }
    else {
        cout << endl;
    }

    cout << string(60, '=') << endl;
    cout << "\n按任意键返回菜单..." << endl;
    cin.ignore();
    cin.get();
}

int main() {
    // 初始化数据
    vector<EquipmentData> helmets = initializeHelmets();
    vector<EquipmentData> armors = initializeArmors();

    while (true) {
        clearScreen();

        cout << "=== 装备维修计算器 ===" << endl;
        cout << "请选择要计算的装备类型：" << endl;
        cout << "1. 头盔维修计算" << endl;
        cout << "2. 护甲维修计算" << endl;
        cout << "0. 退出程序" << endl;

        int typeChoice;
        cout << "\n请输入选择编号 (0-2): ";
        cin >> typeChoice;

        if (typeChoice == 0) {
            clearScreen();
            cout << "感谢使用装备维修计算器，再见！" << endl;
            break;
        }

        if (typeChoice == 1) {
            // 头盔计算
            while (true) {
                int helmetChoice = displayEquipmentMenu(helmets, "头盔");

                if (helmetChoice == 0) {
                    break; // 返回上级菜单
                }

                if (helmetChoice < 1 || helmetChoice >(int)helmets.size()) {
                    cout << "错误：选择编号无效！按任意键继续..." << endl;
                    cin.ignore();
                    cin.get();
                    continue;
                }

                // 执行维修计算
                performRepairCalculation(helmets[helmetChoice - 1]);
            }
        }
        else if (typeChoice == 2) {
            // 护甲计算
            clearScreen();

            if (armors.empty()) {
                cout << "=== 护甲维修计算 ===" << endl;
                cout << "\n抱歉，护甲数据暂未添加，敬请期待！" << endl;
                cout << "\n按任意键返回主菜单..." << endl;
                cin.ignore();
                cin.get();
            }
            else {
                while (true) {
                    int armorChoice = displayEquipmentMenu(armors, "护甲");

                    if (armorChoice == 0) {
                        break; // 返回上级菜单
                    }

                    if (armorChoice < 1 || armorChoice >(int)armors.size()) {
                        cout << "错误：选择编号无效！按任意键继续..." << endl;
                        cin.ignore();
                        cin.get();
                        continue;
                    }

                    // 执行维修计算
                    performRepairCalculation(armors[armorChoice - 1]);
                }
            }
        }
        else {
            cout << "错误：选择编号无效！按任意键继续..." << endl;
            cin.ignore();
            cin.get();
            continue;
        }
    }

    return 0;
}