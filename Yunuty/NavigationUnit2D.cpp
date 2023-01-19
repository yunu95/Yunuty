#include "YunutyEngine.h"

using namespace YunutyEngine;

// �ڽ��� ���� �׺���̼� �ʵ忡 �׺���̼� ������ 
void NavigationUnit2D::ReportStatus()
{
    auto navField = NavigationField2D::GetAssignedField(GetTransform()->GetWorldPosition());
    if (navField)
    {
        auto currentReport = NavigationUnit2DReport(this);
        navField->ReceiveUnitStatus(currentReport, lastReport);
        lastReport = currentReport;
    }
}
