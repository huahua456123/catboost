#include "modes.h"

#include <catboost/libs/init/init_reg.h>
#include <catboost/libs/logging/logging.h>
#include <catboost/libs/app_helpers/mode_calc_helpers.h>

#include <library/svnversion/svnversion.h>
#include <library/getopt/small/modchooser.h>

static int mode_calc(int argc, const char** argv) {
    THolder<NCB::IModeCalcImplementation> modeCalcImplementaion;
    if (NCB::TModeCalcImplementationFactory::Has(NCB::EImplementationType::YandexSpecific)) {
        modeCalcImplementaion = NCB::TModeCalcImplementationFactory::Construct(NCB::EImplementationType::YandexSpecific);
    } else {
        CB_ENSURE(NCB::TModeCalcImplementationFactory::Has(NCB::EImplementationType::OpenSource),
            "Mode calc implementation factory should have open source implementation");
        modeCalcImplementaion = NCB::TModeCalcImplementationFactory::Construct(NCB::EImplementationType::OpenSource);
    }
    return modeCalcImplementaion->mode_calc(argc, argv);
}

int main(int argc, const char* argv[]) {
    try {
        NCB::TCmdLineInit::Do(argc, argv);

        SetVerboseLogingMode();
        TModChooser modChooser;
        modChooser.AddMode("fit", mode_fit, "train model");
        modChooser.AddMode("calc", mode_calc, "evaluate model predictions");
        modChooser.AddMode("fstr", mode_fstr, "evaluate feature importances");
        modChooser.AddMode("ostr", mode_ostr, "evaluate object importances");
        modChooser.AddMode("eval-metrics", mode_eval_metrics, "evaluate metrics for model");
        modChooser.AddMode("metadata", mode_metadata, "get/set/dump metainfo fields from model");
        modChooser.AddMode("run-worker", mode_run_worker, "run worker");
        modChooser.AddMode("roc", mode_roc, "evaluate data for roc curve");
        modChooser.DisableSvnRevisionOption();
        modChooser.SetVersionHandler(PrintProgramSvnVersion);
        return modChooser.Run(argc, argv);
    } catch (...) {
        Cerr << "AN EXCEPTION OCCURRED. " << CurrentExceptionMessage() << Endl;
        return EXIT_FAILURE;
    }
}
