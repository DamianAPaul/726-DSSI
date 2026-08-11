#include "AudioObjects.h"
AudioControlSGTL5000 audioShield;


// GUItool: begin automatically generated code
AudioSynthWaveform       waveform2;      //xy=126.38095092773438,216.71428871154785
AudioSynthWaveform       waveform3;      //xy=128.14286041259766,366.2857303619385
AudioSynthWaveform       waveform8;      //xy=125.14285278320312,1245.4286060333252
AudioSynthWaveform       waveform4;      //xy=129.3809585571289,547.0952033996582
AudioSynthWaveform       waveform5;      //xy=129.3809585571289,715.2857303619385
AudioSynthWaveform       waveform1;      //xy=136.5714340209961,32.42857074737549
AudioSynthWaveform       waveform6;      //xy=133.42858505249023,890.1667613983154
AudioSynthWaveform       waveform7;      //xy=132.99999237060547,1065.2619171142578
AudioFilterStateVariable filter22;       //xy=307.5000114440918,1185.0000162124634
AudioFilterStateVariable filter5;        //xy=311.6666717529297,233.33332061767578
AudioFilterStateVariable filter7;        //xy=311.6666793823242,340.00000381469727
AudioFilterStateVariable filter6;        //xy=313.3333206176758,288.3333396911621
AudioFilterStateVariable filter1;        //xy=316.28571701049805,30.85714340209961
AudioFilterStateVariable filter9;        //xy=315.00000762939453,453.3333225250244
AudioFilterStateVariable filter21;       //xy=312.50001525878906,1117.500020980835
AudioFilterStateVariable filter4;        //xy=316.6666717529297,180.00000190734863
AudioFilterStateVariable filter3;        //xy=317.14287185668945,129.99999809265137
AudioFilterStateVariable filter8;        //xy=316.6666946411133,398.3333282470703
AudioFilterStateVariable filter17;       //xy=315.00000762939453,882.5000152587891
AudioFilterStateVariable filter10;       //xy=316.6666793823242,503.3333625793457
AudioFilterStateVariable filter18;       //xy=315.0000114440918,940.0000190734863
AudioFilterStateVariable filter2;        //xy=318.5714340209961,80
AudioFilterStateVariable filter19;       //xy=315.00001525878906,1002.5000152587891
AudioFilterStateVariable filter20;       //xy=315.0000152587892,1057.5000171661377
AudioFilterStateVariable filter15;       //xy=316.66667556762695,766.6666374206543
AudioFilterStateVariable filter16;       //xy=317.50000762939453,830.0000133514404
AudioFilterStateVariable filter13;       //xy=319.99999237060547,656.6666526794434
AudioFilterStateVariable filter14;       //xy=320.0000457763672,708.333324432373
AudioFilterStateVariable filter11;       //xy=321.66668128967285,558.3333568572998
AudioFilterStateVariable filter12;       //xy=321.6666564941406,610.0000095367432
AudioFilterStateVariable filter23;       //xy=322.50001525878906,1240.0000190734863
AudioFilterStateVariable filter24;       //xy=327.50000762939453,1307.500020980835
AudioMixer4              mixer8;         //xy=530.0000190734863,355.0000171661377
AudioMixer4              mixer7;         //xy=540.0000152587891,287.50001525878906
AudioMixer4              mixer9;         //xy=547.5000228881836,442.5000190734863
AudioMixer4              mixer6;         //xy=552.5000190734863,212.50001525878906
AudioMixer4              mixer4;         //xy=557.5000076293945,80.0000171661377
AudioMixer4              mixer5;         //xy=557.5000381469727,145.0000171661377
AudioMixer4              mixer10;        //xy=560.0000152587891,515.0000228881836
AudioMixer4              mixer11;        //xy=575.0000076293945,597.500022649765
AudioEffectEnvelope      envelope5;      //xy=694.2857360839844,287.99999237060547
AudioEffectEnvelope      envelope6;      //xy=694.2857360839844,334.99999237060547
AudioEffectEnvelope      envelope7;      //xy=694.2857360839844,380.99999237060547
AudioEffectEnvelope      envelope3;      //xy=695.2857360839844,192.99999237060547
AudioEffectEnvelope      envelope4;      //xy=695.2857360839844,242.99999237060547
AudioEffectEnvelope      envelope1;      //xy=696.2857360839844,86.99999237060547
AudioEffectEnvelope      envelope2;      //xy=696.2857360839844,138.99999237060547
AudioEffectEnvelope      envelope8;      //xy=696.2857360839844,434.99999237060547
AudioMixer4              mixer1;         //xy=857.2857360839844,160.99999237060547
AudioMixer4              mixer2;         //xy=859.2857360839844,350.99999237060547
AudioMixer4              mixer3;         //xy=1010.2857360839844,254.99999237060547
AudioEffectChorus        chorus1;
AudioEffectFlange        flange1;
AudioEffectFreeverb      reverb1;
AudioEffectDelay         delay1;
AudioEffectBitcrusher    bitcrusher1;
AudioEffectGranular      granular1;
AudioMixer4              mixer12;
AudioMixer4              mixer13;
AudioMixer4              mixer14;
AudioAmplifier           amp1;           //xy=1168.7857360839844,259.74999237060547
AudioOutputI2S           i2s1;           //xy=1374.5079040527344,254.97220611572266
AudioConnection          patchCord1(waveform2, 0, filter4, 0);
AudioConnection          patchCord2(waveform2, 0, filter5, 0);
AudioConnection          patchCord3(waveform2, 0, filter6, 0);
AudioConnection          patchCord4(waveform3, 0, filter7, 0);
AudioConnection          patchCord5(waveform3, 0, filter8, 0);
AudioConnection          patchCord6(waveform3, 0, filter9, 0);
AudioConnection          patchCord7(waveform8, 0, filter22, 0);
AudioConnection          patchCord8(waveform8, 0, filter23, 0);
AudioConnection          patchCord9(waveform8, 0, filter24, 0);
AudioConnection          patchCord10(waveform4, 0, filter10, 0);
AudioConnection          patchCord11(waveform4, 0, filter11, 0);
AudioConnection          patchCord12(waveform4, 0, filter12, 0);
AudioConnection          patchCord13(waveform5, 0, filter13, 0);
AudioConnection          patchCord14(waveform5, 0, filter14, 0);
AudioConnection          patchCord15(waveform5, 0, filter15, 0);
AudioConnection          patchCord16(waveform1, 0, filter1, 0);
AudioConnection          patchCord17(waveform1, 0, filter2, 0);
AudioConnection          patchCord18(waveform1, 0, filter3, 0);
AudioConnection          patchCord19(waveform6, 0, filter16, 0);
AudioConnection          patchCord20(waveform6, 0, filter17, 0);
AudioConnection          patchCord21(waveform6, 0, filter18, 0);
AudioConnection          patchCord22(waveform7, 0, filter19, 0);
AudioConnection          patchCord23(waveform7, 0, filter20, 0);
AudioConnection          patchCord24(waveform7, 0, filter21, 0);
AudioConnection          patchCord25(filter22, 0, mixer11, 0);
AudioConnection          patchCord26(filter5, 1, mixer5, 1);
AudioConnection          patchCord27(filter7, 0, mixer6, 0);
AudioConnection          patchCord28(filter6, 2, mixer5, 2);
AudioConnection          patchCord29(filter1, 0, mixer4, 0);
AudioConnection          patchCord30(filter9, 2, mixer6, 2);
AudioConnection          patchCord31(filter21, 2, mixer10, 2);
AudioConnection          patchCord32(filter4, 0, mixer5, 0);
AudioConnection          patchCord33(filter3, 2, mixer4, 2);
AudioConnection          patchCord34(filter8, 1, mixer6, 1);
AudioConnection          patchCord35(filter17, 1, mixer9, 1);
AudioConnection          patchCord36(filter10, 0, mixer7, 0);
AudioConnection          patchCord37(filter18, 2, mixer9, 2);
AudioConnection          patchCord38(filter2, 1, mixer4, 1);
AudioConnection          patchCord39(filter19, 0, mixer10, 0);
AudioConnection          patchCord40(filter20, 1, mixer10, 1);
AudioConnection          patchCord41(filter15, 2, mixer8, 2);
AudioConnection          patchCord42(filter16, 0, mixer9, 0);
AudioConnection          patchCord43(filter13, 0, mixer8, 0);
AudioConnection          patchCord44(filter14, 1, mixer8, 1);
AudioConnection          patchCord45(filter11, 1, mixer7, 1);
AudioConnection          patchCord46(filter12, 2, mixer7, 2);
AudioConnection          patchCord47(filter23, 1, mixer11, 1);
AudioConnection          patchCord48(filter24, 2, mixer11, 2);
AudioConnection          patchCord49(mixer8, envelope5);
AudioConnection          patchCord50(mixer7, envelope4);
AudioConnection          patchCord51(mixer9, envelope6);
AudioConnection          patchCord52(mixer6, envelope3);
AudioConnection          patchCord53(mixer4, envelope1);
AudioConnection          patchCord54(mixer5, envelope2);
AudioConnection          patchCord55(mixer10, envelope7);
AudioConnection          patchCord56(mixer11, envelope8);
AudioConnection          patchCord57(envelope5, 0, mixer2, 0);
AudioConnection          patchCord58(envelope6, 0, mixer2, 1);
AudioConnection          patchCord59(envelope7, 0, mixer2, 2);
AudioConnection          patchCord60(envelope3, 0, mixer1, 2);
AudioConnection          patchCord61(envelope4, 0, mixer1, 3);
AudioConnection          patchCord62(envelope1, 0, mixer1, 0);
AudioConnection          patchCord63(envelope2, 0, mixer1, 1);
AudioConnection          patchCord64(envelope8, 0, mixer2, 3);
AudioConnection          patchCord65(mixer1, 0, mixer3, 1);
AudioConnection          patchCord66(mixer2, 0, mixer3, 2);
AudioConnection          patchCord67(mixer3, 0, mixer12, 0);
AudioConnection          patchCord68(amp1, 0, i2s1, 0);
AudioConnection          patchCord69(amp1, 0, i2s1, 1);
AudioConnection          patchCord70(mixer3, chorus1);
AudioConnection          patchCord71(mixer3, reverb1);
AudioConnection          patchCord72(mixer3, delay1);
AudioConnection          patchCord73(mixer3, bitcrusher1);
AudioConnection          patchCord74(mixer3, granular1);
AudioConnection          patchCord75(chorus1, 0, mixer12, 1);
AudioConnection          patchCord76(reverb1, 0, mixer12, 2);
AudioConnection          patchCord77(delay1, 0, mixer12, 3);
AudioConnection          patchCord78(bitcrusher1, 0, mixer13, 0);
AudioConnection          patchCord79(granular1, 0, mixer13, 1);
AudioConnection          patchCord80(mixer12, 0, mixer14, 0);
AudioConnection          patchCord81(mixer13, 0, mixer14, 1);
AudioConnection          patchCord82(mixer14, amp1);
AudioConnection          patchCord83(mixer3, flange1);
AudioConnection          patchCord84(flange1, 0, mixer13, 2);
// GUItool: end automatically generated code
