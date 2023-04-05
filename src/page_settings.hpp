#ifndef PAGE_SETTINGS_H
#define PAGE_SETTINGS_H
#include <QMetaType>
class ParametresPage
{

public:
    int page_num;
    int left_page_number;
    int right_page_number;
    int TypePage;

    ParametresPage(int np, int npg, int npd, int tp){
        page_num = np;
        left_page_number = npg;
        right_page_number = npd;
        TypePage = tp;
    }
    ParametresPage(){
        page_num = 0;
        left_page_number = 0;
        right_page_number = 0;
        TypePage = 0;
    }
};
Q_DECLARE_METATYPE(ParametresPage);

#endif // PAGE_SETTINGS_H
