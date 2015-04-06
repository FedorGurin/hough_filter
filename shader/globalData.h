#ifndef GLOBALDATA_H
#define GLOBALDATA_H

typedef struct
{
    //! ���������� ������������� �������
    int id;

    //! ��� �������
    int code;
    //! ���� �����
    float alfa;
    //! ��������� ��������
    float vc;

    //! ���� ���������� ������� �������� � ����������� ��
    float fi;
    float unt;

    //! ���������� �������� � �������������� ��
    float fi_geo;
    float lam_geo;

    //! ���������� ������ �������� � �������������� ��
    float fi0_geo;
    float lam0_geo;

    //! ���������� � ������ ��
    float xg;
    float yg;
    float zg;

    //! ���������� ������� �������� ������������� ������ ��
    float vxg;
    float vyg;
    float vzg;

    //! ���������� ���������
    float axg;
    float ayg;
    float azg;

    //! ������� �������� �������� ��������
    float omega_x;
    float omega_y;
    float omega_z;

    //! ���������� �������� � ��������� ��
    float psi;
    float gamma;
    float tan;
    //! ����� ����
    float m;
    //!
    float nya;
    //!
    float ny;
    //!
    float nx;

    int idManevr;
}TVis;
//! ������ �� ������ � �������.
typedef struct _THeight_data
{
    double x;   // ���������� ����� � ������� ������������� ������ �������.
    double z;   // ���������� ����� � ������� ������������� ������ �������.
    double h;   // ������ �����.
}THeight_data;


#endif // GLOBALDATA_H
