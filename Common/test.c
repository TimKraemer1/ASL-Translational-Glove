#include "Node.h"
#include "Board.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    BOARD_Init();
    

    Node_t dummy = DefaultInit();
    Node_t t_f = DefaultInit();
    Node_t t_s = DefaultInit();
    AddChild(&dummy, &t_f);
    AddChild(&dummy, &t_s);
    SetFlex(&t_f, FLEXED);
    SetFlex(&t_s, STRAIGHT);

    Node_t i_ff = DefaultInit();
    Node_t i_fs = DefaultInit();
    AddChild(&t_f, &i_ff);
    AddChild(&t_f, &i_fs);
    SetFlex(&i_ff, FLEXED);
    SetFlex(&i_fs, STRAIGHT);

    Node_t i_sf = DefaultInit();
    Node_t i_ss = DefaultInit();
    AddChild(&t_s, &i_sf);
    AddChild(&t_s, &i_ss);
    SetValue(&i_sf, "10");
    SetFlex(&i_sf, FLEXED);
    SetFlex(&i_ss, STRAIGHT);

    Node_t m_fsf = DefaultInit();
    Node_t m_fss = DefaultInit();
    Node_t m_ssf = DefaultInit();
    Node_t m_sff = DefaultInit();
    Node_t m_ffs = DefaultInit();
    Node_t m_fff = DefaultInit();
    AddChild(&i_fs, &m_fsf);
    AddChild(&i_fs, &m_fss);
    AddChild(&i_ss, &m_ssf);
    AddChild(&i_sf, &m_sff);
    AddChild(&i_ff,&m_ffs);
    AddChild(&i_ff,&m_fff);
    SetFlex(&m_fsf, FLEXED);
    SetFlex(&m_fss, STRAIGHT);
    SetFlex(&m_ssf,FLEXED);
    SetFlex(&m_sff,FLEXED);
    SetFlex(&m_ffs,STRAIGHT);
    SetFlex(&m_fff,FLEXED);
    SetLetter(&m_ffs, "F");
    SetValue(&m_ffs, "9");
    SetLetter(&m_sff, "Y");
    SetLetter(&m_ssf, "G");

    Node_t m_sss = DefaultInit();
    AddChild(&i_ss, &m_sss);
    SetFlex(&m_sss, STRAIGHT);

    Node_t r_fsff = DefaultInit();
    Node_t r_ffff = DefaultInit();
    Node_t r_fsfs = DefaultInit();
    AddChild(&m_fsf, &r_fsff);
    AddChild(&m_fsf, &r_fsfs);
    AddChild(&m_fff,&r_ffff);
    SetFlex(&r_fsff, FLEXED);
    SetFlex(&r_fsfs, STRAIGHT);
    SetFlex(&r_ffff, FLEXED);
    SetValue(&r_fsff, "1");
    SetLetter(&r_fsff, "D");
    SetValue(&r_fsfs, "8");

    Node_t r_fssf = DefaultInit();
    Node_t r_fsss = DefaultInit();
    AddChild(&m_fss, &r_fssf);
    AddChild(&m_fss, &r_fsss);
    SetFlex(&r_fssf, FLEXED);
    SetFlex(&r_fsss, STRAIGHT);

    Node_t r_sssf = DefaultInit();
    Node_t r_ssss = DefaultInit();
    AddChild(&m_sss, &r_sssf);
    AddChild(&m_sss, &r_ssss);
    SetFlex(&r_sssf, FLEXED);
    SetFlex(&r_ssss, STRAIGHT);
    SetValue(&r_sssf, "3");
    SetLetter(&r_sssf, "H");
    SetValue(&r_ssss, "5");

    Node_t p_fssff = DefaultInit();
    Node_t p_fssfs = DefaultInit();
    AddChild(&r_fssf, &p_fssff);
    AddChild(&r_fssf, &p_fssfs);
    SetFlex(&p_fssff, FLEXED);
    SetFlex(&p_fssfs, STRAIGHT);
    SetValue(&p_fssff, "2");
    SetLetter(&p_fssff,"R");
    SetValue(&p_fssfs, "7");

    Node_t p_fsssf = DefaultInit();
    Node_t p_fssss = DefaultInit();
    Node_t p_fffff = DefaultInit();
    Node_t p_ffffs = DefaultInit();
    AddChild(&r_fsss, &p_fsssf);
    AddChild(&r_fsss, &p_fssss);
    AddChild(&r_ffff,&p_fffff);
    AddChild(&r_ffff,&p_ffffs);
    SetFlex(&p_fsssf, FLEXED);
    SetFlex(&p_fssss, STRAIGHT);
    SetFlex(&p_fffff,FLEXED);
    SetFlex(&p_ffffs,STRAIGHT);
    SetLetter(&p_fffff, "O");
    SetValue(&p_fffff, "0");
    SetValue(&p_fsssf, "6");
    SetLetter(&p_fsssf, "W");
    SetValue(&p_fssss, "4");
    SetLetter(&p_fssss,"B");
    SetLetter(&p_ffffs, "I");

    Flexion_t positions[5] = {1, 2, 1, 1, 1};

    printf("%s\r\n", GetVal(dummy, positions,1));
    return 0;
}
