include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackSystem
QT += script network
equals(QT_MAJOR_VERSION, 5) {
QT += widgets multimedia multimediawidgets network
OTHER_FILES += system.json
}
unix:QT += dbus

include(systeminfo/systeminfo.pri)

HEADERS += actionpacksystem.h \
    systemsession.h
RESOURCES += actionpacksystem.qrc
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
win32:LIBS += -lPowrProf

TRANSLATIONS = ../../locale/actionpacksystem_fr_FR.ts \
                ../../locale/actionpacksystem_de_DE.ts

SOURCES += \
    systemsession.cpp

unix {
        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../../locale/actionpacksystem_fr_FR.qm \
                        ../../locale/actionpacksystem_de_DE.qm
	locales.CONFIG = no_check_exist

	INSTALLS += locales
}
