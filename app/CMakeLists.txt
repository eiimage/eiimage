
set(eiimage_SOURCES
	main.cpp
	Tools.h
	Algorithms/ClassAnalysis.cpp
	Algorithms/ClassAnalysis.cpp
	Algorithms/ClassAnalysis.h
	Algorithms/Croissance.cpp
	Algorithms/Croissance.cpp
	Algorithms/Croissance.h
	Algorithms/DCT.cpp
	Algorithms/DCT.cpp
	Algorithms/DCT.h
	Algorithms/FFT.cpp
	Algorithms/FFT.cpp
	Algorithms/FFT.h
	Algorithms/Pyramid.cpp
	Algorithms/Pyramid.cpp
	Algorithms/Pyramid.h
	Operations/BFlitOp.cpp
	Operations/BFlitOp.h
	Operations/CenterOp.cpp
	Operations/CenterOp.h
	Operations/ClassAnalysisDialog.cpp
	Operations/ClassAnalysisDialog.h
	Operations/ClassAnalysisOp.cpp
	Operations/ClassAnalysisOp.h
	Operations/ClassResultOp.cpp
	Operations/ClassResultOp.h
	Operations/ColorDialog.cpp
	Operations/ColorDialog.h
	Operations/ColorimetryOp.cpp
	Operations/ColorimetryOp.h
	Operations/CombineColorOp.cpp
	Operations/CombineColorOp.h
	Operations/CroissanceOp.cpp
	Operations/CroissanceOp.h
	Operations/DCTDialog.cpp
	Operations/DCTDialog.h
	Operations/DCTOp.cpp
	Operations/DCTOp.h
	Operations/DMMDialog.cpp
	Operations/DMMDialog.h
	Operations/DMMOp.cpp
	Operations/DMMOp.h
	Operations/EntropyOp.cpp
	Operations/EntropyOp.h
	Operations/FFTDialog.cpp
	Operations/FFTDialog.h
	Operations/FFTOp.cpp
	Operations/FFTOp.h
	Operations/FlipOp.cpp
	Operations/FlipOp.h
	Operations/HadamardOp.cpp
	Operations/HadamardOp.h
	Operations/HistogramOp.cpp
	Operations/HistogramOp.h
	Operations/HoughDialog.cpp
	Operations/HoughDialog.cpp
	Operations/HoughDialog.h
	Operations/HoughOp.cpp
	Operations/HoughOp.cpp
	Operations/HoughOp.h
	Operations/Huffman.cpp
	Operations/Huffman.h
	Operations/HuffmanOp.cpp
	Operations/HuffmanOp.h
	Operations/IFFTOp.cpp
	Operations/IFFTOp.h
	Operations/InverseHoughDialog.cpp
	Operations/InverseHoughDialog.cpp
	Operations/InverseHoughDialog.h
	Operations/InverseHoughOp.cpp
	Operations/InverseHoughOp.cpp
	Operations/InverseHoughOp.h
	Operations/InversePyramidDialog.cpp
	Operations/InversePyramidDialog.h
	Operations/InversePyramidOp.cpp
	Operations/InversePyramidOp.h
	Operations/MeanSquaredErrorOp.cpp
	Operations/MeanSquaredErrorOp.h
	Operations/MICD.cpp
	Operations/MICD.h
	Operations/MICDDialog.cpp
	Operations/MICDDialog.h
	Operations/MICDEncodingOp.cpp
	Operations/MICDEncodingOp.h
	Operations/NoiseOp.cpp
	Operations/NoiseOp.h
	Operations/PointOp.cpp
	Operations/PointOp.h
	Operations/PseudoColorOp.cpp
	Operations/PseudoColorOp.h
	Operations/PyramidDialog.cpp
	Operations/PyramidDialog.h
	Operations/PyramidOp.cpp
	Operations/PyramidOp.h
	Operations/Quantification.cpp
	Operations/Quantification.h
	Operations/QuantificationDialog.cpp
	Operations/QuantificationDialog.h
	Operations/QuantificationOp.cpp
	Operations/QuantificationOp.h
	Operations/QuantificationWidget.cpp
	Operations/QuantificationWidget.h
	Operations/RandomImgOp.cpp
	Operations/RandomImgOp.h
	Operations/RejectionRingOp.cpp
	Operations/RejectionRingOp.h
	Operations/RotateOp.cpp
	Operations/RotateOp.h
	Operations/ScalingOp.cpp
	Operations/ScalingOp.h
	Operations/SeparatorOp.cpp
	Operations/SeparatorOp.h
	Operations/SignalToNoiseOp.cpp
	Operations/SignalToNoiseOp.h
	Operations/SinusSynthesisOp.cpp
	Operations/SinusSynthesisOp.cpp
	Operations/SinusSynthesisOp.h
	Operations/SplitColorOp.cpp
	Operations/SplitColorOp.h
	Operations/ThresholdDialog.cpp
	Operations/ThresholdDialog.cpp
	Operations/ThresholdDialog.h
	Operations/ThresholdOp.cpp
	Operations/ThresholdOp.cpp
	Operations/ThresholdOp.h
	Operations/Transforms.cpp
	Operations/Transforms.h
	Operations/TranslateOp.cpp
	Operations/TranslateOp.h
	Operations/ZeroCrossingOp.cpp
	Operations/ZeroCrossingOp.h
	Services/EIImageService.cpp
	Services/EIImageService.h
	Services/FilteringService.cpp
	Services/FilteringService.h
	Services/MorphoMatService.cpp
	Services/MorphoMatService.cpp
	Services/MorphoMatService.h
	Services/OperationService.cpp
	Services/OperationService.h
	Services/OpSetService.cpp
	Services/OpSetService.h
	Services/PluginManager.cpp
	Services/PluginManager.h
	Widgets/FilterChoice.cpp
	Widgets/FilterChoice.cpp
	Widgets/FilterChoice.h
	Widgets/FilterEditor.cpp
	Widgets/FilterEditor.h
	Widgets/FilterEditorItem.cpp
	Widgets/FilterEditorItem.h
	Widgets/ImageZoneSelector.cpp
	Widgets/ImageZoneSelector.h
	Widgets/OperationBar.cpp
	Widgets/OperationBar.h
	Widgets/StructElemViewer.cpp
	Widgets/StructElemViewer.h
	Widgets/StructElemWindow.cpp
	Widgets/StructElemWindow.h
)

set(UIS
	Operations/HoughDialog.ui
	Operations/InverseHoughDialog.ui
	Operations/FFTDialog.ui
	Operations/ClassAnalysisDialog.ui
	Operations/InversePyramidDialog.ui
	Operations/PyramidDialog.ui
	Operations/MICDDialog.ui
	Operations/DCTDialog.ui
	Operations/ColorDialog.ui
	Operations/MICDDialog_.ui
)

qt5_wrap_ui(WRAPPED_UIS ${UIS})

qt5_add_resources(RCC_FILES "eiimage_app_res.qrc")

set(TS_FILES eiimage_en.ts eiimage_fr.ts)
# Uncommento only 1 of the 2 folowing commands, depending on your needs
# -1- Use current translations files
qt5_add_translation(QM_FILES ${TS_FILES})
# -2- Update translations files with changes from c++ sources
# qt5_create_translation(QM_FILES ${eiimage_SOURCES} ${TS_FILES})

# Apply only on MinGW / Cygwin under Windows
if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
	# Don't require libstdc++**.dll when running the program
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
endif()

add_executable(eiimage ${eiimage_SOURCES} ${WRAPPED_UIS} ${UIS} ${TS_FILES} ${RCC_FILES})
target_link_libraries(eiimage
	core
	Qt5::Core
	Qt5::Widgets
	Qt5::Gui
	Qt5::Script
	Qt5::Xml
	${RandomLib_LIBRARIES}
	GenericInterface
	ImageIn
)
