//------------------------------------------------------------------------------
// Header generated by wxDesigner from file: muuli.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#ifndef __WDR_muuli_H__
#define __WDR_muuli_H__

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "muuli_wdr.h"
#endif

// Include wxWindows' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/statline.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/toolbar.h>

// Declare window functions

#define ID_TEXT 10000
#define ID_SYSTRAYSELECT 10001
#define ID_OK 10002
wxSizer *desktopDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *s_dlgcnt;
extern wxSizer *buttonToolbar;
extern wxSizer *contentSizer;
extern wxSizer *s_fed2klh;
#define ID_TEXTCTRL 10003
#define ID_BUTTON_FAST 10004
#define ID_LINE 10005
#define ID_STATICBITMAP 10006
wxSizer *muleDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SRV_SPLITTER 10007
wxSizer *serverListDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *s_searchdlgsizer;
extern wxSizer *s_searchsizer;
#define IDC_SEARCHNAME 10008
#define IDC_SEARCH_RESET 10009
#define ID_SEARCHTYPE 10010
#define ID_EXTENDEDSEARCHCHECK 10011
extern wxSizer *s_extendedsizer;
#define IDC_TypeSearch 10012
#define ID_AUTOCATASSIGN 10013
#define IDC_EDITSEARCHEXTENSION 10014
#define IDC_SPINSEARCHMIN 10015
#define IDC_SPINSEARCHMAX 10016
#define IDC_SPINSEARCHAVAIBILITY 10017
#define IDC_STARTS 10018
#define IDC_CANCELS 10019
#define IDC_SDOWNLOAD 10020
#define IDC_CLEAR_RESULTS 10021
#define ID_NOTEBOOK 10022
#define ID_SEARCHPROGRESS 10023
wxSizer *searchDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *transfer_top_boxsizer;
#define ID_BTNCLRCOMPL 10024
#define ID_CATEGORIES 10025
#define ID_BTNSWWINDOW 10026
#define ID_DLOADLIST 10027
wxSizer *transferTopPane( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *queueSizer;
#define ID_BTNSWITCHUP 10028
#define ID_QUEUELIST 10029
#define ID_UPLOADLIST 10030
wxSizer *transferBottomPane( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_FRIENDLIST 10031
#define IDC_CHATSELECTOR 10032
#define IDC_CMESSAGE 10033
#define IDC_CSEND 10034
#define IDC_CCLOSE 10035
wxSizer *messagePage( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *IDC_FD_X0;
#define IDC_FD_X1 10036
#define IDC_FNAME 10037
#define IDC_FD_X2 10038
#define IDC_METFILE 10039
#define IDC_FD_X3 10040
#define IDC_FHASH 10041
#define IDC_FD_X4 10042
#define IDC_FSIZE 10043
#define IDC_FD_X5 10044
#define IDC_PFSTATUS 10045
#define IDC_FD_X15 10046
#define IDC_LASTSEENCOMPL 10047
extern wxSizer *IDC_MINF_0;
#define IDC_MINF_1 10048
#define IDC_MINFCODEC 10049
#define IDC_MINF_2 10050
#define IDC_MINFBIT 10051
#define IDC_MINF_3 10052
#define IDC_MINFLEN 10053
extern wxSizer *IDC_FD_X6;
#define IDC_FD_X7 10054
#define IDC_SOURCECOUNT 10055
#define IDC_FD_X10 10056
#define IDC_FD_X9 10057
#define IDC_PARTCOUNT 10058
#define IDC_FD_X11 10059
#define IDC_FD_X13 10060
#define IDC_DATARATE 10061
#define IDC_FD_X14 10062
#define IDC_TRANSFERED 10063
#define IDC_FD_X12 10064
#define IDC_SOURCECOUNT2 10065
#define IDC_PARTAVAILABLE 10066
#define IDC_COMPLSIZE 10067
#define IDC_PROCCOMPL 10068
extern wxSizer *IDC_FD_ICH;
#define IDC_FD_LSTATS1 10069
#define IDC_FD_STATS1 10070
#define IDC_FD_LSTATS2 10071
#define IDC_FD_STATS2 10072
#define IDC_FD_LSTATS3 10073
#define IDC_FD_STATS3 10074
extern wxSizer *IDC_FD_SN;
#define IDC_LISTCTRLFILENAMES 10075
#define IDC_TAKEOVER 10076
#define IDC_BUTTONSTRIP 10077
#define IDC_FILENAME 10078
#define IDC_RENAME 10079
#define ID_CLOSEWNDFD 5100
#define IDC_CMTBT 10080
wxSizer *fileDetails( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_CMT_TEXT 10081
#define IDC_FC_CLEAR 10082
#define IDC_RATELIST 10083
#define IDCOK 10084
#define IDCCANCEL 10085
wxSizer *commentDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_LST 10086
#define IDC_CMSTATUS 10087
#define IDCREF 10088
wxSizer *commentLstDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_ANIMATE 10089
#define ID_CANCEL 5101
wxSizer *downloadDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_IPADDRESS 10090
#define ID_IPORT 10091
#define ID_USERNAME 10092
#define ID_USERHASH 10093
#define ID_ADDFRIEND 10094
#define ID_CLOSEDLG 10095
wxSizer *addFriendDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_BTNRELSHARED 10096
#define ID_SHFILELIST 10097
#define IDC_SREQUESTED 10098
#define ID_GAUGE 10099
#define IDC_SREQUESTED2 10100
#define IDC_SACCEPTED 10101
#define IDC_SACCEPTED2 10102
#define IDC_STRANSFERED 10103
#define IDC_STRANSFERED2 10104
wxSizer *sharedfilesDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *testSizer;
#define ID_DLOADSCOPE 10105
#define IDC_C0 10106
#define IDC_C0_3 10107
#define IDC_C0_2 10108
#define ID_OTHERS 10109
#define IDC_S3 10110
#define IDC_S0 10111
#define ID_ACTIVEC 10112
#define IDC_S1 10113
#define ID_ULOADSCOPE 10114
#define IDC_C1 10115
#define IDC_C1_3 10116
#define IDC_C1_2 10117
#define ID_TREECTRL 10118
wxSizer *statsDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_DNAME 10119
#define ID_DHASH 10120
#define ID_DSOFT 10121
#define ID_DIP 10122
#define ID_DSIP 10123
#define ID_DVERSION 10124
#define ID_DID 10125
#define ID_DSNAME 10126
#define ID_DDOWNLOADING 10127
#define ID_DDOWN 10128
#define ID_DAVDR 10129
#define ID_DDOWNTOTAL 10130
#define ID_DDUP 10131
#define ID_DAVUR 10132
#define ID_DUPTOTAL 10133
#define ID_DRATIO 10134
#define ID_DRATING 10135
#define IDC_CDIDENT 10136
#define ID_DSCORE 10137
#define ID_CLOSEWND 10138
wxSizer *clientDetails( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_NICK 10139
#define IDC_LANGUAGE 10140
#define IDC_DBLCLICK 10141
#define IDC_MINTRAY 10142
#define IDC_EXIT 10143
#define IDC_TOOLTIPDELAY_LBL 10144
#define IDC_TOOLTIPDELAY 10145
#define ID_DESKTOPMODE 10146
#define IDC_STARTMIN 10147
#define IDC_FCHECK 10148
#define ID_CUSTOMBROWSETEXT 10149
#define IDC_FCHECKSELF 10150
#define IDC_FCHECKTABS 10151
wxSizer *PreferencesGeneralTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_DLIMIT_LBL 10152
#define IDC_MAXDOWN 10153
#define IDC_KBS1 10154
#define IDC_ULIMIT_LBL 10155
#define IDC_MAXUP 10156
#define IDC_KBS4 10157
#define IDC_SLOTALLOC 10158
#define IDC_DCAP_LBL 10159
#define IDC_DOWNLOAD_CAP 10160
#define IDC_KBS2 10161
#define IDC_UCAP_LBL 10162
#define IDC_UPLOAD_CAP 10163
#define IDC_KBS3 10164
#define IDC_PORT 10165
#define IDC_UDPPORT 10166
#define IDC_UDPDISABLE 10167
#define IDC_MAXSRCHARD_LBL 10168
#define IDC_MAXSOURCEPERFILE 10169
#define IDC_MAXCONLABEL 10170
#define IDC_MAXCON 10171
#define IDC_AUTOCONNECT 10172
#define IDC_RECONN 10173
#define IDC_SHOWOVERHEAD 10174
wxSizer *PreferencesConnectionTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_REMOVEDEAD 10175
#define IDC_SERVERRETRIES 10176
#define IDC_RETRIES_LBL 10177
#define IDC_AUTOSERVER 10178
#define IDC_EDITADR 10179
#define IDC_FILTER 10180
#define IDC_UPDATESERVERCONNECT 10181
#define IDC_UPDATESERVERCLIENT 10182
#define IDC_SCORE 10183
#define IDC_SMARTIDCHECK 10184
#define IDC_SAFESERVERCONNECT 10185
#define IDC_AUTOCONNECTSTATICONLY 10186
#define IDC_MANUALSERVERHIGHPRIO 10187
wxSizer *PreferencesServerTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ICH 10188
#define IDC_AICHTRUST 10189
#define IDC_ADDNEWFILESPAUSED 10190
#define IDC_DAP 10191
#define IDC_PREVIEWPRIO 10192
#define IDC_UAP 10193
#define IDC_FULLCHUNKTRANS 10194
#define IDC_STARTNEXTFILE 10195
#define IDC_SRCSEEDS 10196
#define IDC_METADATA 10197
#define IDC_CHUNKALLOC 10198
#define IDC_FULLALLOCATE 10199
#define IDC_CHECKDISKSPACE 10200
#define ID_MINDISKTEXT 10201
#define IDC_MINDISKSPACE 10202
wxSizer *PreferencesFilesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_INCFILES 10203
#define IDC_SELINCDIR 10204
#define IDC_TEMPFILES 10205
#define IDC_SELTEMPDIR 10206
#define IDC_SHARESELECTOR 10207
#define IDC_VIDEOPLAYER 10208
#define IDC_BROWSEV 10209
#define IDC_VIDEOBACKUP 10210
wxSizer *PreferencesDirectoriesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_SLIDERINFO 10211
#define IDC_SLIDER 10212
#define IDC_SLIDERINFO3 10213
#define IDC_SLIDER3 10214
#define IDC_SLIDERINFO4 10215
#define IDC_SLIDER4 10216
#define IDC_PREFCOLORS 10217
#define IDC_COLORSELECTOR 10218
#define IDC_COLOR_BUTTON 10219
#define IDC_SLIDERINFO2 10220
#define IDC_SLIDER2 10221
wxSizer *PreferencesStatisticsTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_CB_TBN_USESOUND 10222
#define IDC_EDIT_TBN_WAVFILE 10223
#define IDC_BTN_BROWSE_WAV 10224
#define IDC_CB_TBN_ONLOG 10225
#define IDC_CB_TBN_ONCHAT 10226
#define IDC_CB_TBN_POP_ALWAYS 10227
#define IDC_CB_TBN_ONDOWNLOAD 10228
#define IDC_CB_TBN_ONNEWVERSION 10229
#define IDC_CB_TBN_IMPORTATNT 10230
#define IDC_SENDMAIL 10231
#define IDC_SMTP 10232
#define IDC_EMAIL 10233
wxSizer *PreferencesNotifyTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_WARNING 10234
#define IDC_STATIC 10235
#define IDC_MAXCON5SECLABEL 10236
#define IDC_MAXCON5SEC 10237
#define IDC_SAFEMAXCONN 10238
#define IDC_VERBOSE 10239
#define IDC_VERBOSEPACKETERROR 10240
#define IDC_FILEBUFFERSIZE_STATIC 10241
#define IDC_FILEBUFFERSIZE 10242
#define IDC_QUEUESIZE_STATIC 10243
#define IDC_QUEUESIZE 10244
#define IDC_SERVERKEEPALIVE_LABEL 10245
#define IDC_SERVERKEEPALIVE 10246
wxSizer *PreferencesaMuleTweaksTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_PERCENT 10247
#define IDC_PROGBAR 10248
#define IDC_3DDEP 10249
#define IDC_3DDEPTH 10250
#define IDC_FLAT 10251
#define IDC_ROUND 10252
#define IDC_USESKIN 10253
#define IDC_SKINFILE 10254
#define IDC_SELSKINFILE 10255
#define IDC_FED2KLH 10256
#define IDC_EXTCATINFO 10257
#define IDC_SHOWRATEONTITLE 10258
wxSizer *PreferencesGuiTweaksTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_NNS_HANDLING 10259
#define IDC_ENABLE_AUTO_FQS 10260
#define IDC_ENABLE_AUTO_HQRS 10261
#define IDC_HQR_VALUE 10262
#define IDC_AUTO_DROP_TIMER 10263
wxSizer *PreferencesSourcesDroppingTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ENABLE_WEB 10264
#define IDC_WEB_PORT 10265
#define IDC_WEB_REFRESH_TIMEOUT 10266
#define IDC_WEB_GZIP 10267
#define IDC_ENABLE_WEB_LOW 10268
#define IDC_WEB_PASSWD 10269
#define IDC_WEB_PASSWD_LOW 10270
#define IDC_EXT_CONN_ACCEPT 10271
#define IDC_EXT_CONN_USETCP 10272
#define IDC_EXT_CONN_TCP_PORT 10273
#define IDC_EXT_CONN_PASSWD_ENABLE 10274
#define IDC_EXT_CONN_PASSWD 10275
wxSizer *PreferencesRemoteControlsTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *prefs_main_sizer;
extern wxSizer *prefs_sizer;
extern wxSizer *prefs_list_sizer;
#define ID_PREFSLISTCTRL 10276
#define ID_PREFS_OK_TOP 10277
#define ID_PREFS_CANCEL_TOP 10278
wxSizer *preferencesDlgTop( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_STATIC_TITLE 10279
#define IDC_TITLE 10280
#define IDC_STATIC_COMMENT 10281
#define IDC_COMMENT 10282
#define IDC_STATIC_INCOMING 10283
#define IDC_INCOMING 10284
#define IDC_BROWSE 10285
#define IDC_STATIC_PRIO 10286
#define IDC_PRIOCOMBO 10287
#define IDC_STATIC_COLOR 10288
#define ID_BOX_CATCOLOR 10289
#define IDC_CATCOLOR 10290
wxSizer *CategoriesEditWindow( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SPLATTER 10291
wxSizer *transferDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SERVERINFO 10292
#define ID_BTN_RESET_SERVER 10293
wxSizer *ServerInfoLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_LOGVIEW 10294
#define ID_BTN_RESET 10295
wxSizer *aMuleLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_UPDATELIST 10296
#define IDC_SERVERLISTURL 10297
#define IDC_SERVERNAME 10298
#define IDC_IPADDRESS 10299
#define IDC_SPORT 10300
#define ID_ADDTOLIST 10301
#define ID_SERVERLIST 10302
wxSizer *serverListDlgUp( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SRVLOG_NOTEBOOK 10303
wxSizer *serverListDlgDown( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_LOCALETEXT 10304
#define ID_LOCALENEVERAGAIN 10305
wxSizer *LocaleWarning( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NODELIST 10306
#define ID_NODE_IP1 10307
#define ID_NODE_IP2 10308
#define ID_NODE_IP3 10309
#define ID_NODE_IP4 10310
#define ID_NODE_PORT 10311
#define ID_NODECONNECT 10312
#define ID_KNOWNNODECONNECT 10313
#define ID_KADSEARCHLIST 10314
wxSizer *KadDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_MYSERVINFO 10315
wxSizer *MyInfoLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_SEESHARES 10316
#define IDC_SPIN_PERM_FU 10317
#define IDC_SPIN_PERM_FG 10318
#define IDC_SPIN_PERM_FO 10319
#define IDC_SPIN_PERM_DU 10320
#define IDC_SPIN_PERM_DG 10321
#define IDC_SPIN_PERM_DO 10322
#define IDC_IPFONOFF 10323
#define IDC_IPFRELOAD 10324
#define IDC_AUTOIPFILTER 10325
#define IDC_IPFILTERURL 10326
#define IDC_IPFILTERUPDATE 10327
#define ID_SPINCTRL 10328
#define IDC_SECIDENT 10329
wxSizer *PreferencesSecurityTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ONLINESIG 10330
#define IDC_OSDIR 10331
#define IDC_SELOSDIR 10332
wxSizer *PreferencesOnlineSigTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

// Declare menubar functions

// Declare toolbar functions

#define ID_BUTTONCONNECT 10333
#define ID_BUTTONSERVERS 10334
#define ID_BUTTONKAD 10335
#define ID_BUTTONSEARCH 10336
#define ID_BUTTONTRANSFER 10337
#define ID_BUTTONSHARED 10338
#define ID_BUTTONMESSAGES 10339
#define ID_BUTTONSTATISTICS 10340
#define ID_BUTTONNEWPREFERENCES 10341
#define ID_ABOUT 10342
void muleToolbar( wxToolBar *parent );

// Declare bitmap functions

wxBitmap clientImages( size_t index );

wxBitmap dlStatusImages( size_t index );

wxBitmap connImages( size_t index );

wxBitmap moreImages( size_t index );

wxBitmap amuleSpecial( size_t index );

wxBitmap connButImg( size_t index );

wxBitmap amuleDlgImages( size_t index );

#endif

// End of generated file
