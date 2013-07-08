//---------------------------------------------------------------------------

#ifndef ParticleFiliterSimuH
#define ParticleFiliterSimuH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OmniDirectionalImg.h"
#include "ProbabilityEvaluation.h"
#include "CommonUnit.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
private:	    // User declarations
public:		    // User declarations
    __fastcall TForm1(TComponent* Owner);

    TInfo *info;
    TLocProbEvaluation *LocProbEvaluation;
    TSimuOmniDrctImg *SimuOmniDrctImg;

    Graphics::TBitmap *Map;
    unsigned char* MapImgPtr;

    Graphics::TBitmap *Source;
    unsigned char* SourceImgPtr;

    Graphics::TBitmap *OutPut;
    unsigned char* OutPutImgPtr;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
