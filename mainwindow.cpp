#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDate>
#include    <QTableWidgetItem>
#include    <QRandomGenerator>


//================== 点名册数据（取自《周四点名册.xls》，课程：Qt应用程序开发，任课教师：刘文果） ==================
//作业要求：取"本人学号"在点名册中的前两行、本行、后两行，共5行填入表格
const QString kMyStuID="2024414290205"; //本人的学号：所在行的"学号""姓名"单元格显示为加粗红色

//每行8个字段：学号、姓名、性别、行政班级、院系、专业、修读性质、籍贯
//性别：按姓名推测均填"男"，如有错误请改；另外4位同学的籍贯未知，暂留空（选中该行时状态栏显示"（未知）"）
const RollItem kRollData[5]={
    {"2024414290202","陈向泉","男","2024软件2班","计算机科学与技术学院（软件学院、网络空间安全学院）","软件工程","正常修读",""},
    {"2024414290203","陈宇衡","男","2024软件2班","计算机科学与技术学院（软件学院、网络空间安全学院）","软件工程","正常修读",""},
    {"2024414290205","成远煜","男","2024软件2班","计算机科学与技术学院（软件学院、网络空间安全学院）","软件工程","正常修读","广东省东莞市"},
    {"2024414290206","程迪宏","男","2024软件2班","计算机科学与技术学院（软件学院、网络空间安全学院）","软件工程","正常修读",""},
    {"2024414290207","邓云飞","男","2024软件2班","计算机科学与技术学院（软件学院、网络空间安全学院）","软件工程","正常修读",""},
};
//==========================================================================================================


//为一行的单元格创建 Items
void MainWindow::createItemsARow(int rowNo,QString name,QString sex,QDate birth,QString nation,bool isPM,int score)
{
    uint studID=202105000;  //学号基数
    //姓名
    QTableWidgetItem *item=new  QTableWidgetItem(name, MainWindow::ctName);   //数据项类型为MainWindow::ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    studID  +=rowNo;        //学号 =基数 + 行号
    item->setData(Qt::UserRole,QVariant(studID));           //设置studID为用户数据
    ui->tableInfo->setItem(rowNo,MainWindow::colName,item);

    //性别
    QIcon   icon;
    if (sex=="男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");

    item=new  QTableWidgetItem(sex,MainWindow::ctSex);      //type为MainWindow::ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Qt::ItemFlags flags=Qt::ItemIsSelectable |Qt::ItemIsEnabled;    //不允许编辑
    item->setFlags(flags);
    ui->tableInfo->setItem(rowNo,MainWindow::colSex,item);  //为单元格设置Item

    //出生日期
    QString str=birth.toString("yyyy-MM-dd");   //日期转换为字符串
    item=new  QTableWidgetItem(str,MainWindow::ctBirth);        //type为MainWindow::ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);   //文本对齐格式
    ui->tableInfo->setItem(rowNo,MainWindow::colBirth,item);

    //民族
    item=new  QTableWidgetItem(nation,MainWindow::ctNation);        //type为MainWindow::ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colNation,item);

    //是否党员
    item=new  QTableWidgetItem("党员",MainWindow::ctPartyM);      //type为 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    flags= Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled;   //不允许编辑，但可以更改复选状态
    item->setFlags(flags);
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow));   //设置背景颜色
    ui->tableInfo->setItem(rowNo,MainWindow::colPartyM,item);

    //分数
    str.setNum(score);
    item=new  QTableWidgetItem(str,MainWindow::ctScore);    //type为MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colScore,item);
}

//按点名册数据填充表格的一行（isMe为true表示是本人所在行："学号""姓名"单元格加粗红色）
void MainWindow::createRollRow(int rowNo, const RollItem &stu, bool isMe)
{
    //学号
    QTableWidgetItem *item=new QTableWidgetItem(stu.id);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colStuID,item);

    //姓名：在Item的用户数据(UserRole)里关联学生的籍贯，选中该行时在状态栏显示
    item=new QTableWidgetItem(stu.name,MainWindow::ctName); //type为MainWindow::ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setData(Qt::UserRole,QVariant(stu.native));       //籍贯存入用户数据
    ui->tableInfo->setItem(rowNo,MainWindow::colRollName,item);

    //性别
    item=new QTableWidgetItem(stu.sex);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colRollSex,item);

    //行政班级
    item=new QTableWidgetItem(stu.cls);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colClass,item);

    //院系
    item=new QTableWidgetItem(stu.dept);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colDept,item);

    //专业
    item=new QTableWidgetItem(stu.major);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colMajor,item);

    //修读性质
    item=new QTableWidgetItem(stu.mode);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colMode,item);

    if (isMe) //本人所在行："学号""姓名"单元格设置为粗体、红色
    {
        QFont fontBold;             //粗体字体
        fontBold.setBold(true);

        item=ui->tableInfo->item(rowNo,MainWindow::colStuID); //学号单元格
        item->setFont(fontBold);
        item->setForeground(QBrush(Qt::red));   //文字颜色为红色

        item=ui->tableInfo->item(rowNo,MainWindow::colRollName); //姓名单元格
        item->setFont(fontBold);
        item->setForeground(QBrush(Qt::red));
    }
}

//工具栏QToolButton(动作actLoadRoll)的槽函数：点击后重新设置右侧tableWidget的表头和内容
void MainWindow::on_actLoadRoll_triggered()
{
    //1.重新设置表头：学号、姓名、性别、行政班级、院系、专业、修读性质
    QStringList headerText;
    headerText<<"学号"<<"姓名"<<"性别"<<"行政班级"<<"院系"<<"专业"<<"修读性质";
    ui->tableInfo->clearContents();                     //只清除工作区，不清除表头
    ui->tableInfo->setColumnCount(headerText.size());   //设置表格列数
    for (int i=0;i<headerText.size();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();      //获取原有字体设置
        font.setBold(true);                 //表头设置为粗体
        headerItem->setFont(font);          //设置字体
        headerItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableInfo->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的item
    }

    //2.重新设置表格内容：本人学号的前两行 + 本行 + 后两行，共5行
    int myRow=-1;                           //本人所在的行号
    ui->tableInfo->setRowCount(5);
    for (int i=0;i<5;i++)
    {
        bool isMe=(kRollData[i].id==kMyStuID);
        if (isMe) myRow=i;
        createRollRow(i,kRollData[i],isMe); //为某一行创建items
    }

    //3.选中本人所在行，状态栏立即显示本人的籍贯信息
    ui->tableInfo->resizeColumnsToContents();   //列宽适应内容
    if (myRow>=0)
    {
        ui->tableInfo->selectRow(myRow);
        ui->tableInfo->setCurrentCell(myRow,0);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    setCentralWidget(ui->splitterMain);

    //状态栏初始化创建
    labCellIndex = new QLabel("当前单元格坐标：",this);
    labCellIndex->setMinimumWidth(550);

    labCellType=new QLabel("当前单元格类型：",this);
    labCellType->setMinimumWidth(200);

    labStudID=new QLabel("学生ID：",this);
    labStudID->setMinimumWidth(200);

    labNative=new QLabel("籍贯：",this);    //用于显示选中学生的籍贯
    labNative->setMinimumWidth(200);

    ui->statusBar->addWidget(labCellIndex); //添加到状态栏
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudID);
    ui->statusBar->addWidget(labNative);

    on_actLoadRoll_triggered(); //启动时自动载入点名册数据
}

MainWindow::~MainWindow()
{
    delete ui;
}


//设置水平表头
void MainWindow::on_btnSetHeader_clicked()
{
    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    //    ui->tableInfo->setHorizontalHeaderLabels(headerText); //只设置标题
    ui->tableInfo->setColumnCount(headerText.size());      //设置表格列数
    for (int i=0;i<ui->tableInfo->columnCount();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();   //获取原有字体设置
        font.setBold(true);              //设置为粗体
        font.setPointSize(11);           //字体大小
        headerItem->setForeground(QBrush(Qt::red));  //设置文字颜色
        headerItem->setFont(font);       //设置字体
        ui->tableInfo->setHorizontalHeaderItem(i,headerItem);    //设置表头单元格的item
    }
}

//设置行数,设置的行数为数据区的行数，不含表头
void MainWindow::on_btnSetRows_clicked()
{
    ui->tableInfo->setRowCount(ui->spinRowCount->value());//设置数据区行数
    ui->tableInfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked()); //设置交替行背景颜色
}


//初始化表格数据
void MainWindow::on_btnIniData_clicked()
{
    QDate   birth(2001,4,6);        //初始化一个日期
    ui->tableInfo->clearContents(); //只清除工作区，不清除表头
    for (int i=0; i<ui->tableInfo->rowCount(); i++)
    {
        QString strName=QString("学生%1").arg(i);
        QString strSex= ((i % 2)==0)? "男":"女";
        bool isParty= ((i % 2)==0)? false:true;
        int score= QRandomGenerator::global()->bounded(60,100);   //随机数[60,100)
        createItemsARow(i, strName, strSex, birth,"汉族",isParty,score);  //为某一行创建items
        birth=birth.addDays(20);    //日期加20天
    }
}

void MainWindow::on_chkBoxTabEditable_clicked(bool checked)
{ //设置表格是否可编辑，以及进入编辑模式的方式
    if (checked)
        //双击或获取焦点后单击，进入编辑状态
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked
                                       | QAbstractItemView::SelectedClicked);
    else
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers); //不允许编辑
}

void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{//是否显示水平表头
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{//是否显示垂直表头
    ui->tableInfo->verticalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{ //行的底色交替采用不同颜色
    ui->tableInfo->setAlternatingRowColors(checked);
}

void MainWindow::on_rBtnSelectItem_clicked()
{//选择方式：单元格选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void MainWindow::on_rBtnSelectRow_clicked()
{//选择方式：行选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}


//将 QTableWidget的所有行的内容提取字符串，显示在QPlainTextEdit里
void MainWindow::on_btnReadToEdit_clicked()
{
//    QString str;
    QTableWidgetItem    *item;

    ui->textEdit->clear();  //文本编辑器清空
    for (int i=0;i<ui->tableInfo->rowCount();i++)   //逐行处理
    {
        QString str=QString::asprintf("第 %d 行： ",i+1);
        for (int j=0;j<ui->tableInfo->columnCount()-1;j++) //逐列处理，但最后一列是check型，单独处理
        {
            item=ui->tableInfo->item(i,j);      //获取单元格的item
            str=str+item->text()+"   ";         //字符串连接
        }
        item=ui->tableInfo->item(i,colPartyM);  //最后一列，党员
        if (item->checkState()==Qt::Checked)    //根据check状态显示文字
            str=str+"党员";
        else
            str=str+"群众";
        ui->textEdit->appendPlainText(str);     //添加到编辑框作为一行
    }
}

//currentCellChanged()信号的槽函数，当前单元格发生变化时的响应
void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    QTableWidgetItem* item=ui->tableInfo->item(currentRow,currentColumn); //获取单元格的Item
    if  (item==nullptr)
        return;

    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行，%d 列",currentRow,currentColumn));

    int cellType=item->type();      //获取单元格的类型
    labCellType->setText(QString::asprintf("当前单元格类型：%d",cellType));

    item=ui->tableInfo->item(currentRow,MainWindow::colStuID); //取当前行第1列(学号列)的单元格的item
    if (item!=nullptr)
    {
        uint ID=item->data(Qt::UserRole).toUInt();      //读取用户数据(旧演示数据中为学号ID)
        if (ID!=0)
            labStudID->setText(QString::asprintf("学生ID：%d",ID)); //旧演示数据
        else
            labStudID->setText("学号："+item->text());   //点名册数据：第1列的文本即学号
    }

    showNativePlace(currentRow);    //在状态栏显示当前行学生的籍贯信息
}

//插入一行
void MainWindow::on_btnInsertRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();     //当前行号

    ui->tableInfo->insertRow(curRow);           //插入一行，但不会自动为单元格创建item
    createItemsARow(curRow, "新学生", "男",
                    QDate::fromString("2002-10-1","yyyy-M-d"),"苗族",true,80 ); //为某一行创建items
}

//添加一行
void MainWindow::on_btnAppendRow_clicked()
{
    int curRow=ui->tableInfo->rowCount();       //当前行号
    ui->tableInfo->insertRow(curRow);           //在表格尾部添加一行
    createItemsARow(curRow, "新生", "女",
                    QDate::fromString("2002-6-5","yyyy-M-d"),"满族",false,76 ); //为某一行创建items
}

//删除当前行及其items
void MainWindow::on_btnDelCurRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();     //当前行号
    ui->tableInfo->removeRow(curRow);           //删除当前行及其items
}

void MainWindow::on_btnAutoHeght_clicked()
{
    ui->tableInfo->resizeRowsToContents();
}

void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableInfo->resizeColumnsToContents();
}

//在状态栏的labNative标签上显示第row行学生的籍贯
//（籍贯信息关联在"姓名"单元格Item的用户数据Qt::UserRole里）
void MainWindow::showNativePlace(int row)
{
    if (row<0 || row>=ui->tableInfo->rowCount())    //没有有效的行
    {
        labNative->setText("籍贯：");
        return;
    }

    for (int col=0;col<ui->tableInfo->columnCount();col++) //逐列查找"姓名"单元格(type为ctName)
    {
        QTableWidgetItem *item=ui->tableInfo->item(row,col);
        if (item!=nullptr && item->type()==MainWindow::ctName)
        {
            QVariant native=item->data(Qt::UserRole);   //读取姓名Item里关联的籍贯信息
            if (native.typeId()==QMetaType::QString)    //点名册数据：UserRole里存的是籍贯(QString)
            {
                QString place=native.toString();
                if (place.isEmpty())                    //该学生籍贯未知
                    place="（未知）";
                labNative->setText("籍贯："+place);
            }
            else                                        //旧演示数据：UserRole里存的是学号ID
                labNative->setText("籍贯：");
            return;
        }
    }
    labNative->setText("籍贯：");
}

//选中行发生变化时的槽函数：在状态栏显示被选中行学生的籍贯信息
void MainWindow::on_tableInfo_itemSelectionChanged()
{
    int row=ui->tableInfo->currentRow();        //默认取当前单元格所在的行
    QList<QModelIndex> rows=ui->tableInfo->selectionModel()->selectedRows(); //行选择模式下取被选中的行
    if (!rows.isEmpty())
        row=rows.last().row();
    showNativePlace(row);
}
