import sys

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import (QWidget, QMainWindow, QLabel, QLineEdit,
                             QComboBox, QGridLayout, QApplication, QPushButton)

from core import Core


class SingletonUI(QMainWindow):
    sinExit = pyqtSignal()

    def __init__(self):
        super().__init__()

        self.init_ui()
        self.init_layout()

    def init_layout(self):
        self.set_msg('就绪')

        grid = QGridLayout()
        grid.setSpacing(25)

        grid.addWidget(self.hwnd, 1, 0)
        grid.addWidget(self.hwnd_edit, 1, 1)

        grid.addWidget(self.keyword, 2, 0)
        grid.addWidget(self.keyword_edit, 2, 1)

        grid.addWidget(self.interval, 3, 0)
        grid.addWidget(self.interv_combo, 3, 1)

        grid.addWidget(self.email, 4, 0)
        grid.addWidget(self.email_edit, 4, 1)

        grid.addWidget(self.ok_btn, 5, 0, 1, 2)

        self.setFixedSize(360, 270)
        self.setWindowTitle('腻害的东东')

        widget = QWidget()
        widget.setLayout(grid)

        self.setCentralWidget(widget)
        self.show()

    def init_ui(self):
        self.hwnd = QLabel('监控程序')
        self.keyword = QLabel('关键词语')
        self.interval = QLabel('扫描间隔')
        self.email = QLabel('邮件地址')

        self.hwnd_edit = QLineEdit()
        self.hwnd_edit.setPlaceholderText('被监控程序标题包含,如:QQ三国')
        self.hwnd_edit.setText('QQ三国')

        self.keyword_edit = QLineEdit()
        self.keyword_edit.setPlaceholderText('窗口内包含此关键词,如:掉线')
        self.keyword_edit.setText('断线')

        self.email_edit = QLineEdit()
        self.email_edit.setPlaceholderText('接收通知的邮件地址,多个用|分隔')
        self.email_edit.setText('x_jwei@qq.com|382324131@qq.com')

        self.interv_combo = QComboBox()
        self.interv_combo.addItem("15s")
        self.interv_combo.addItem("30s")
        self.interv_combo.addItem("60s")
        self.interv_combo.addItem("120s")
        self.interv_combo.setCurrentIndex(2)
        self.interv_combo.activated[int].connect(self.onActivated)

        self.ok_btn = QPushButton("开始")
        self.ok_btn.setCheckable(False)
        self.ok_btn.clicked.connect(self.onOkClick)

    def onOkClick(self, is_start):
        self.ok_btn.setCheckable(not is_start)
        if is_start:
            self.sinExit.emit()
            self.ok_btn.setEnabled(False)
            self.set_msg('可能需要等待{}'.format(self.interv_combo.currentText()))
            # self.thread.sinMsg.disconnect(self.slot_msg)
            del self.thread
        else:
            self.thread = Core(
                self, self.hwnd_edit.text(),
                self.keyword_edit.text(),
                2 ** self.interv_combo.currentIndex() * 15,
                self.email_edit.text()
            )
            self.thread.sinMsg.connect(self.slot_msg)
            self.thread.start()
            self.ok_btn.setText('停止')

    def onActivated(self, index):
        self.interv_combo.setCurrentIndex(index)
        self.set_msg('当前扫描间隔: ' + self.interv_combo.currentText())

    def set_msg(self, msg):
        self.statusBar().showMessage(msg)

    def slot_msg(self, msg):
        if msg != "Release:2019/12/1":
            self.set_msg(msg)
        else:
            self.set_msg('上个任务已结束,可重新开始')
            self.ok_btn.setText('开始')
            self.ok_btn.setEnabled(True)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    instance = SingletonUI()
    sys.exit(app.exec_())
