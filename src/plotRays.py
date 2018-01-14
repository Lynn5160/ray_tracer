from PySide2 import QtWidgets

clipboard = QtWidgets.QApplication.clipboard()
data = clipboard.text()
l = data.split(",")

def plotRay(l):
    ox, oy, oz, dx, dy, dz = 0, 1, 2, 3, 4, 5
    groupList = []
    for i in range(0, len(l)/6):
        cmds.curve(p=[(l[ox], l[oy], l[oz]), (l[dx], l[dy], l[dz])])
        groupList.append(cmds.ls(sl=1)[0])
        ox += 6
        oy += 6
        oz += 6
        dx += 6
        dy += 6
        dz += 6
    cmds.group(groupList, n="raysGroup", w=1)


def plotVec(l):
    x, y, z = 0, 1, 2
    groupList = []
    for i in range(0, len(l) / 3):
        cmds.spaceLocator(p=[l[x], l[y], l[z]])
        shape = cmds.ls(dag=1,s=1,sl=1)[0]
        for i in ['X', 'Y', 'Z']:
            cmds.setAttr(shape + ".localScale" + i, 0.01)
            groupList.append(cmds.ls(sl=1)[0])
        x += 3
        y += 3
        z += 3
    cmds.group(groupList, n="vectorsGroup", w=1)

plotVec(l)
plotRay(l)

