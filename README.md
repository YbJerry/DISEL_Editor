# DISEL_Editor

### Introduction
DISEL Editor is a software for editing DISEL(Domain Information System Extended Language). 
It can be used by users which are not familiar with DISEL to create their own ontology. 

### Build
Building DISEL Editor needs these tools:
* [Qt 6](https://www.qt.io/)
* [DISELAPI](https://github.com/YbJerry/DISELAPI/)
* [Graphviz](https://graphviz.org/)
* [TinyXML-2](http://leethomason.github.io/tinyxml2/)

#### Qt
Look at the website of Qt and download Qt 6. Click [diselgui.pro](https://github.com/YbJerry/DISEL_Editor/blob/main/diselgui.pro) and build it.

#### DISELAPI
Download and compile DISELAPI. We use static link to import DISELAPI library. And you need to change the following information in [diselgui.pro](https://github.com/YbJerry/DISEL_Editor/blob/main/diselgui.pro).

```
win32:CONFIG(release, debug|release): LIBS += -L'YOUR RELEASE VERSION LIBRARY PATH' -lDISELAPILIB
else:win32:CONFIG(debug, debug|release): LIBS += -L'YOUR DEBUG VERSION LIBRARY PATH' -lDISELAPILIB
else:unix: LIBS += -L'YOUR LIBRARY PATH' -lDISELAPILIB


INCLUDEPATH += YOUR HEADER FILE PATH
DEPENDPATH += YOUR HEADER FILE PATH
```

#### Graphviz
Download and install Graphviz. And you need to change the following information in [diselgui.pro](https://github.com/YbJerry/DISEL_Editor/blob/main/diselgui.pro).
```
unix|win32: LIBS += -L'YOUR GRAPHVIZ LIBRARY PATH' -lgvc -lcdt -lcgraph

INCLUDEPATH += 'YOUR GRAPHVIZ HEADER FILE PATH'
DEPENDPATH += 'YOUR GRAPHVIZ HEADER FILE PATH'
```

#### TinyXML-2
Download and compile TinyXML-2 because DISELAPI need it. And you need to change the following information in [diselgui.pro](https://github.com/YbJerry/DISEL_Editor/blob/main/diselgui.pro).
```
win32:CONFIG(release, debug|release): LIBS += -L'YOUR RELEASE VERSION LIBRARY PATH' -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L'YOUR DEBUG VERSION LIBRARY PATH' -ltinyxml2d
else:unix: LIBS += -L'YOUR LIBRARY PATH' -ltinyxml2


INCLUDEPATH += 'YOUR TinyXML-2 HEADER FILE PATH'
DEPENDPATH += 'YOUR TinyXML-2 HEADER FILE PATH'
```

### Pre-build Version
We also provide pre-bulid version of DISEL Editor. You can download it in this page. But for now, we only provide Windows x64 version.

### Help Documentation
You can get information about DISEL Editor in [help documentation website](https://ybjerry.github.io/DISEL_Editor_helpdoc/).
