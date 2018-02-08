#ifndef _CDTL645_H
#define _CDTL645_H
#include "User_Mcu_Cfg.h"
#include "CDTL2007.h"

class CDTL645
{

  public:
	  void   CodeToAck97Data(struct Stu_Dtl2007 &Dtl2007Buf,const INT8U *vSrcBuf,const INT16U vSrcLen);
	  	 	 //-------------------------------------------------
//���������ơ�CodeReadDataCmd(TH_IN const INT32U pSrcDi,TH_IN const INT8U *pSrcAmtAddBuf,TH_IN INT8U type,TH_OUT  sDtl2007_Read &vDstCmdData)��
//���������ܡ�     //���������
//������˵����        
//��I:	���롿        INT32U s_Di;//DI
//                    INT8U  s_AmtBuf[6];//����ַ
//                    INT8U  s_type;//����ģʽ��0����FE�� 1����4��FE                
//��O:	�����       sDtl2007_Read &vDstCmdData
//������ֵ	��     ��ȷ��DACK_SUCESS   ����DACK_FAILURE   
//��ȫ�ֱ�����       
//������˵����           
//��������	��  ���   �����ڡ� 2016��8��01��
//����  ������ ԭʼ�汾
//-------------------------------------------------
//���޸���	��     �����ڡ�
//����  ������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	  INT8U CodeReadData97Cmd(TH_IN const INT16U pSrcDi,TH_IN const INT8U *pSrcAmtAddBuf,TH_IN INT8U type,TH_OUT  sDtl2007_Read &vDstCmdData);
	  //-------------------------------------------------
     //INT8U  AdjustTimeCode(const INT8U *buf,INT8U pSrcMaxLen,INT8U *tempbuf,INT8U &pDstLen,INT8U type=0);
     INT8U  CheckAmtAdd97(const INT8U *pSrcAdd,const INT8U *pSrcRcvAdd);
     void   CodeReadData97(const INT16U pSrcDi,const INT8U *pSrcAmtAddBuf,INT8U *pDstBuf,INT8U &pDstLen,INT8U type=0);
     INT8U  Decode64597(const INT8U *pSrcBuf,const INT16U pSrcLen,const INT8U vSrcDataLen,INT8U *pDstAmtAddBuf,INT16U &pDstDi,INT8U *pDstBuf,INT8U &pDstLen);
     virtual void  CodeWriteData97(const INT16U pSrcDi,const INT8U *pSrcAmtAddBuf,const INT8U *pSrcBuf,const INT8U pSrcLen,INT8U *pDstBuf,INT8U &pDstLen,INT8U type=0);
     INT8U CheckFullFrame64597(const INT8U *buf,const INT16U pSrcLen);
     //����û��Fe��ʽ
     INT8U CodeReadDataNoFe97(const INT16U pSrcDi,const INT8U *pSrcAmtAddBuf,INT16U vSrcLen,INT8U *pDstBuf,INT8U &pDstLen);
      //����ת��,07������Ҫת���ɹ���07�����ݸ�ʽ
   //INT8U  Tran97ToGW07(INT16U pSrcDi,INT8U *vDstBuf,INT8U &vDstLen);
   //����ת��,97������Ҫת���ɹ���05�����ݸ�ʽ
   //INT8U  Meter97ToGW05(INT16U pSrcDi,INT8U *vDstBuf,INT8U &vDstLen);
   
    INT8U TranDi97(INT16U vSrcDi16,INT8U vSrcLen,INT16U &vDstDi16,INT8U &vDstLen);
    INT8U ReDecode_GWPCT97(INT16U Di,INT8U len,INT8U *tempbuf,INT8U outlen);
	//����㲥Уʱ����������+��ʱ�ĵĴ���
     //INT8U  Decode645AdjustTime(const INT8U *pSrcBuf,const INT16U pSrcLen,const INT16U vSrcDataLen,INT8U *pDstBuf,INT8U &pDstLen);
	 INT8U CheckOk97(const INT8U *pSrcBuf,const INT8U pSrcLen);
	 void CodeReadAckData97(const INT16U pSrcDi,const INT8U *pSrcAmtAddBuf,const INT8U *pSrcBuf,const INT16U pSrcLen,INT8U *pDstBuf,INT8U &pDstLen,INT8U type);


};
#endif
