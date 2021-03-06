//
// Created by Dmitry Khrykin on 2019-08-17.
//

#ifndef STRATEGR_OPTIONSVIEW_H
#define STRATEGR_OPTIONSVIEW_H

#import "SGCalendarExporter.h"
#import "SGCalendarImportExportView.h"
#import "SGCalendarExportViewModel.h"


NS_ASSUME_NONNULL_BEGIN

@interface SGCalendarExportView : SGCalendarImportExportView <SGCalendarExportViewModelDelegate>

@property(nonatomic) SGCalendarExportViewModel *viewModel;

@end

NS_ASSUME_NONNULL_END

#endif //STRATEGR_OPTIONSVIEW_H
