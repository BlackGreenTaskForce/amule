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
#define IDC_SHOWSTATUSTEXT 10005
#define ID_LINE 10006
#define ID_STATICBITMAP 10007
wxSizer *muleDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SRV_SPLITTER 10008
wxSizer *serverListDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *s_searchdlgsizer;
extern wxSizer *s_searchsizer;
#define IDC_SEARCHNAME 10009
#define IDC_SEARCH_RESET 10010
#define ID_SEARCHTYPE 10011
#define ID_EXTENDEDSEARCHCHECK 10012
extern wxSizer *s_extendedsizer;
#define IDC_TypeSearch 10013
#define ID_AUTOCATASSIGN 10014
#define IDC_EDITSEARCHEXTENSION 10015
#define IDC_SPINSEARCHMIN 10016
#define IDC_SEARCHMINSIZE 10017
#define IDC_SPINSEARCHMAX 10018
#define IDC_SEARCHMAXSIZE 10019
#define IDC_SPINSEARCHAVAIBILITY 10020
#define IDC_STARTS 10021
#define IDC_CANCELS 10022
#define IDC_SDOWNLOAD 10023
#define IDC_CLEAR_RESULTS 10024
#define ID_NOTEBOOK 10025
#define ID_SEARCHPROGRESS 10026
wxSizer *searchDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *transfer_top_boxsizer;
#define ID_BTNCLRCOMPL 10027
#define ID_CATEGORIES 10028
#define ID_BTNSWWINDOW 10029
#define ID_DLOADLIST 10030
wxSizer *transferTopPane( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *queueSizer;
#define ID_CLIENTTOGGLE 10031
#define ID_BTNSWITCHUP 10032
#define ID_CLIENTCOUNT 10033
#define ID_CLIENTLIST 10034
wxSizer *transferBottomPane( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_FRIENDLIST 10035
#define IDC_CHATSELECTOR 10036
#define IDC_CMESSAGE 10037
#define IDC_CSEND 10038
#define IDC_CCLOSE 10039
wxSizer *messagePage( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *IDC_FD_X0;
#define IDC_FD_X1 10040
#define IDC_FNAME 10041
#define IDC_FD_X2 10042
#define IDC_METFILE 10043
#define IDC_FD_X3 10044
#define IDC_FHASH 10045
#define IDC_FD_X4 10046
#define IDC_FSIZE 10047
#define IDC_FD_X5 10048
#define IDC_PFSTATUS 10049
#define IDC_FD_X15 10050
#define IDC_LASTSEENCOMPL 10051
extern wxSizer *IDC_MINF_0;
#define IDC_MINF_1 10052
#define IDC_MINFCODEC 10053
#define IDC_MINF_2 10054
#define IDC_MINFBIT 10055
#define IDC_MINF_3 10056
#define IDC_MINFLEN 10057
extern wxSizer *IDC_FD_X6;
#define IDC_FD_X7 10058
#define IDC_SOURCECOUNT 10059
#define IDC_FD_X10 10060
#define IDC_FD_X9 10061
#define IDC_PARTCOUNT 10062
#define IDC_FD_X11 10063
#define IDC_FD_X13 10064
#define IDC_DATARATE 10065
#define IDC_FD_X14 10066
#define IDC_TRANSFERED 10067
#define IDC_FD_X12 10068
#define IDC_SOURCECOUNT2 10069
#define IDC_PARTAVAILABLE 10070
#define IDC_COMPLSIZE 10071
#define IDC_PROCCOMPL 10072
extern wxSizer *IDC_FD_ICH;
#define IDC_FD_LSTATS1 10073
#define IDC_FD_STATS1 10074
#define IDC_FD_LSTATS2 10075
#define IDC_FD_STATS2 10076
#define IDC_FD_LSTATS3 10077
#define IDC_FD_STATS3 10078
extern wxSizer *IDC_FD_SN;
#define IDC_LISTCTRLFILENAMES 10079
#define IDC_TAKEOVER 10080
#define IDC_BUTTONSTRIP 10081
#define IDC_FILENAME 10082
#define IDC_RENAME 10083
#define ID_CLOSEWNDFD 5100
#define IDC_CMTBT 10084
wxSizer *fileDetails( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_CMT_TEXT 10085
#define IDC_FC_CLEAR 10086
#define IDC_RATELIST 10087
#define IDCOK 10088
#define IDCCANCEL 10089
wxSizer *commentDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_LST 10090
#define IDC_CMSTATUS 10091
#define IDCREF 10092
wxSizer *commentLstDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_ANIMATE 10093
#define ID_HTTPDOWNLOADPROGRESS 10094
#define ID_CANCEL 5101
wxSizer *downloadDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_IPADDRESS 10095
#define ID_IPORT 10096
#define ID_USERNAME 10097
#define ID_USERHASH 10098
#define ID_ADDFRIEND 10099
#define ID_CLOSEDLG 10100
wxSizer *addFriendDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_BTNRELSHARED 10101
#define ID_SHFILELIST 10102
#define IDC_SREQUESTED 10103
#define ID_GAUGE 10104
#define IDC_SREQUESTED2 10105
#define IDC_SACCEPTED 10106
#define IDC_SACCEPTED2 10107
#define IDC_STRANSFERED 10108
#define IDC_STRANSFERED2 10109
wxSizer *sharedfilesDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *testSizer;
#define ID_DLOADSCOPE 10110
#define IDC_C0 10111
#define IDC_C0_3 10112
#define IDC_C0_2 10113
#define ID_OTHERS 10114
#define IDC_S3 10115
#define IDC_S0 10116
#define ID_ACTIVEC 10117
#define IDC_S1 10118
#define ID_ULOADSCOPE 10119
#define IDC_C1 10120
#define IDC_C1_3 10121
#define IDC_C1_2 10122
#define ID_TREECTRL 10123
wxSizer *statsDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_DNAME 10124
#define ID_DHASH 10125
#define ID_DSOFT 10126
#define ID_DIP 10127
#define ID_DSIP 10128
#define ID_DVERSION 10129
#define ID_DID 10130
#define ID_DSNAME 10131
#define ID_DDOWNLOADING 10132
#define ID_DDOWN 10133
#define ID_DAVDR 10134
#define ID_DDOWNTOTAL 10135
#define ID_DDUP 10136
#define ID_DAVUR 10137
#define ID_DUPTOTAL 10138
#define ID_DRATIO 10139
#define ID_DRATING 10140
#define IDC_CDIDENT 10141
#define ID_DSCORE 10142
#define ID_CLOSEWND 10143
wxSizer *clientDetails( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_NICK 10144
#define IDC_LANGUAGE 10145
#define IDC_DBLCLICK 10146
#define IDC_MINTRAY 10147
#define IDC_EXIT 10148
#define IDC_TOOLTIPDELAY_LBL 10149
#define IDC_TOOLTIPDELAY 10150
#define ID_DESKTOPMODE 10151
#define IDC_STARTMIN 10152
#define IDC_FCHECK 10153
#define ID_CUSTOMBROWSETEXT 10154
#define IDC_FCHECKSELF 10155
#define IDC_FCHECKTABS 10156
wxSizer *PreferencesGeneralTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_DLIMIT_LBL 10157
#define IDC_MAXDOWN 10158
#define IDC_KBS1 10159
#define IDC_ULIMIT_LBL 10160
#define IDC_MAXUP 10161
#define IDC_KBS4 10162
#define IDC_SLOTALLOC 10163
#define IDC_DCAP_LBL 10164
#define IDC_DOWNLOAD_CAP 10165
#define IDC_KBS2 10166
#define IDC_UCAP_LBL 10167
#define IDC_UPLOAD_CAP 10168
#define IDC_KBS3 10169
#define IDC_PORT 10170
#define IDC_UDPPORT 10171
#define IDC_UDPDISABLE 10172
#define IDC_MAXSRCHARD_LBL 10173
#define IDC_MAXSOURCEPERFILE 10174
#define IDC_MAXCONLABEL 10175
#define IDC_MAXCON 10176
#define IDC_AUTOCONNECT 10177
#define IDC_RECONN 10178
#define IDC_SHOWOVERHEAD 10179
wxSizer *PreferencesConnectionTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_REMOVEDEAD 10180
#define IDC_SERVERRETRIES 10181
#define IDC_RETRIES_LBL 10182
#define IDC_AUTOSERVER 10183
#define IDC_EDITADR 10184
#define IDC_UPDATESERVERCONNECT 10185
#define IDC_UPDATESERVERCLIENT 10186
#define IDC_SCORE 10187
#define IDC_SMARTIDCHECK 10188
#define IDC_SAFESERVERCONNECT 10189
#define IDC_AUTOCONNECTSTATICONLY 10190
#define IDC_MANUALSERVERHIGHPRIO 10191
wxSizer *PreferencesServerTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ICH 10192
#define IDC_AICHTRUST 10193
#define IDC_ADDNEWFILESPAUSED 10194
#define IDC_DAP 10195
#define IDC_PREVIEWPRIO 10196
#define IDC_UAP 10197
#define IDC_FULLCHUNKTRANS 10198
#define IDC_STARTNEXTFILE 10199
#define IDC_SRCSEEDS 10200
#define IDC_METADATA 10201
#define IDC_CHUNKALLOC 10202
#define IDC_FULLALLOCATE 10203
#define IDC_CHECKDISKSPACE 10204
#define ID_MINDISKTEXT 10205
#define IDC_MINDISKSPACE 10206
wxSizer *PreferencesFilesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_INCFILES 10207
#define IDC_SELINCDIR 10208
#define IDC_TEMPFILES 10209
#define IDC_SELTEMPDIR 10210
#define IDC_SHARESELECTOR 10211
#define IDC_SHAREHIDDENFILES 10212
#define IDC_VIDEOPLAYER 10213
#define IDC_BROWSEV 10214
#define IDC_VIDEOBACKUP 10215
wxSizer *PreferencesDirectoriesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_SLIDERINFO 10216
#define IDC_SLIDER 10217
#define IDC_SLIDERINFO3 10218
#define IDC_SLIDER3 10219
#define IDC_SLIDERINFO4 10220
#define IDC_SLIDER4 10221
#define IDC_PREFCOLORS 10222
#define IDC_COLORSELECTOR 10223
#define IDC_COLOR_BUTTON 10224
#define IDC_SLIDERINFO2 10225
#define IDC_SLIDER2 10226
wxSizer *PreferencesStatisticsTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_CB_TBN_USESOUND 10227
#define IDC_EDIT_TBN_WAVFILE 10228
#define IDC_BTN_BROWSE_WAV 10229
#define IDC_CB_TBN_ONLOG 10230
#define IDC_CB_TBN_ONCHAT 10231
#define IDC_CB_TBN_POP_ALWAYS 10232
#define IDC_CB_TBN_ONDOWNLOAD 10233
#define IDC_CB_TBN_ONNEWVERSION 10234
#define IDC_CB_TBN_IMPORTATNT 10235
#define IDC_SENDMAIL 10236
#define IDC_SMTP 10237
#define IDC_EMAIL 10238
wxSizer *PreferencesNotifyTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_WARNING 10239
#define IDC_STATIC 10240
#define IDC_MAXCON5SECLABEL 10241
#define IDC_MAXCON5SEC 10242
#define IDC_SAFEMAXCONN 10243
#define IDC_VERBOSE 10244
#define IDC_VERBOSEPACKETERROR 10245
#define IDC_FILEBUFFERSIZE_STATIC 10246
#define IDC_FILEBUFFERSIZE 10247
#define IDC_QUEUESIZE_STATIC 10248
#define IDC_QUEUESIZE 10249
#define IDC_SERVERKEEPALIVE_LABEL 10250
#define IDC_SERVERKEEPALIVE 10251
wxSizer *PreferencesaMuleTweaksTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_PERCENT 10252
#define IDC_PROGBAR 10253
#define IDC_3DDEP 10254
#define IDC_3DDEPTH 10255
#define IDC_FLAT 10256
#define IDC_ROUND 10257
#define IDC_USESKIN 10258
#define IDC_SKINFILE 10259
#define IDC_SELSKINFILE 10260
#define IDC_AUTOSORT 10261
#define IDC_FED2KLH 10262
#define IDC_EXTCATINFO 10263
#define IDC_SHOWRATEONTITLE 10264
wxSizer *PreferencesGuiTweaksTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_NNS_HANDLING 10265
#define IDC_ENABLE_AUTO_FQS 10266
#define IDC_ENABLE_AUTO_HQRS 10267
#define IDC_HQR_VALUE 10268
#define IDC_AUTO_DROP_TIMER 10269
wxSizer *PreferencesSourcesDroppingTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ENABLE_WEB 10270
#define IDC_WEB_PORT 10271
#define IDC_WEB_REFRESH_TIMEOUT 10272
#define IDC_WEB_GZIP 10273
#define IDC_ENABLE_WEB_LOW 10274
#define IDC_WEB_PASSWD 10275
#define IDC_WEB_PASSWD_LOW 10276
#define IDC_EXT_CONN_ACCEPT 10277
#define IDC_EXT_CONN_USETCP 10278
#define IDC_EXT_CONN_TCP_PORT 10279
#define IDC_EXT_CONN_PASSWD 10280
wxSizer *PreferencesRemoteControlsTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *prefs_main_sizer;
extern wxSizer *prefs_sizer;
extern wxSizer *prefs_list_sizer;
#define ID_PREFSLISTCTRL 10281
#define ID_PREFS_OK_TOP 10282
#define ID_PREFS_CANCEL_TOP 10283
wxSizer *preferencesDlgTop( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_STATIC_TITLE 10284
#define IDC_TITLE 10285
#define IDC_STATIC_COMMENT 10286
#define IDC_COMMENT 10287
#define IDC_STATIC_INCOMING 10288
#define IDC_INCOMING 10289
#define IDC_BROWSE 10290
#define IDC_STATIC_PRIO 10291
#define IDC_PRIOCOMBO 10292
#define IDC_STATIC_COLOR 10293
#define ID_BOX_CATCOLOR 10294
#define IDC_CATCOLOR 10295
wxSizer *CategoriesEditWindow( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SPLATTER 10296
wxSizer *transferDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SERVERINFO 10297
#define ID_BTN_RESET_SERVER 10298
wxSizer *ServerInfoLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_LOGVIEW 10299
#define ID_BTN_RESET 10300
wxSizer *aMuleLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_UPDATELIST 10301
#define IDC_SERVERLISTURL 10302
#define IDC_SERVERNAME 10303
#define IDC_IPADDRESS 10304
#define IDC_SPORT 10305
#define ID_ADDTOLIST 10306
#define ID_SERVERLIST 10307
wxSizer *serverListDlgUp( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SRVLOG_NOTEBOOK 10308
wxSizer *serverListDlgDown( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_LOCALETEXT 10309
#define ID_LOCALENEVERAGAIN 10310
wxSizer *LocaleWarning( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NODELIST 10311
#define ID_NODE_IP1 10312
#define ID_NODE_IP2 10313
#define ID_NODE_IP3 10314
#define ID_NODE_IP4 10315
#define ID_NODE_PORT 10316
#define ID_NODECONNECT 10317
#define ID_KNOWNNODECONNECT 10318
#define ID_KADSEARCHLIST 10319
wxSizer *KadDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_MYSERVINFO 10320
wxSizer *MyInfoLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_SEESHARES 10321
#define IDC_SPIN_PERM_FU 10322
#define IDC_SPIN_PERM_FG 10323
#define IDC_SPIN_PERM_FO 10324
#define IDC_SPIN_PERM_DU 10325
#define IDC_SPIN_PERM_DG 10326
#define IDC_SPIN_PERM_DO 10327
#define IDC_IPFONOFF 10328
#define IDC_IPFRELOAD 10329
#define IDC_AUTOIPFILTER 10330
#define IDC_IPFILTERURL 10331
#define IDC_IPFILTERUPDATE 10332
#define ID_IPFILTERLEVEL 10333
#define IDC_FILTER 10334
#define IDC_SECIDENT 10335
wxSizer *PreferencesSecurityTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ONLINESIG 10336
#define ID_SPINCTRL 10337
#define IDC_OSDIR 10338
#define IDC_SELOSDIR 10339
wxSizer *PreferencesOnlineSigTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_MSGFILTER 10340
#define IDC_MSGFILTER_ALL 10341
#define IDC_MSGFILTER_NONFRIENDS 10342
#define IDC_MSGFILTER_NONSECURE 10343
#define IDC_MSGFILTER_WORD 10344
#define IDC_MSGWORD 10345
wxSizer *PreferencesMessagesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_CHECKBOX 10346
#define ID_CHOICE 10347
wxSizer *PreferencesProxyTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

// Declare menubar functions

// Declare toolbar functions

#define ID_BUTTONCONNECT 10348
#define ID_BUTTONSERVERS 10349
#define ID_BUTTONKAD 10350
#define ID_BUTTONSEARCH 10351
#define ID_BUTTONTRANSFER 10352
#define ID_BUTTONSHARED 10353
#define ID_BUTTONMESSAGES 10354
#define ID_BUTTONSTATISTICS 10355
#define ID_BUTTONNEWPREFERENCES 10356
#define ID_ABOUT 10357
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
