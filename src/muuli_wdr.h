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

// Declare window functions

#define ID_LOWDOWN 10000
#define ID_MEDIUMDOWN 10001
#define ID_HIGHDOWN 10002
#define ID_TEXT 10003
#define ID_PROVIDER 10004
#define ID_TRUEDOWNLOAD 10005
#define ID_TRUEUPLOAD 10006
#define ID_KBITS 10007
#define ID_KBYTES 10008
#define ID_APPLY 10009
#define ID_CANCEL 10010
wxSizer *connWizDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_GNOME2 10011
#define ID_KDE3 10012
#define ID_KDE2 10013
#define ID_NOSYSTRAY 10014
#define ID_OK 10015
wxSizer *desktopDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *s_dlgcnt;
extern wxSizer *buttonToolbar;
extern wxSizer *contentSizer;
extern wxSizer *s_fed2klh;
#define ID_TEXTCTRL 10016
#define ID_BUTTON_FAST 10017
#define ID_LINE 10018
#define ID_STATICBITMAP 10019
wxSizer *muleDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SRV_SPLITTER 10020
wxSizer *serverListDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *IDC_SEARCH_FRM;
extern wxSizer *s_srcopts;
#define IDC_MSTATIC3 10021
#define IDC_SEARCHNAME 10022
#define IDC_SEARCH_RESET 10023
#define IDC_MSTATIC4 10024
#define IDC_TypeSearch 10025
#define ID_AUTOCATASSIGN 10026
extern wxSizer *s_srced2klh;
#define ID_ED2KLINKHANDLER 10027
#define ID_BTN_DDLOAD 10028
#define IDC_EDITSEARCHMIN 10029
#define IDC_SEARCHMINSIZE 10030
#define IDC_EDITSEARCHMAX 10031
#define IDC_SEARCHMAXSIZE 10032
#define IDC_EDITSEARCHEXTENSION 10033
#define IDC_SEARCHEXTENSION 10034
#define IDC_EDITSEARCHAVAIBILITY 10035
#define IDC_SEARCHAVAIL 10036
#define IDC_SGLOBAL 10037
#define IDC_STARTS 10038
#define IDC_CANCELS 10039
#define IDC_CLEARALL 10040
#define ID_NOTEBOOK 10041
#define ID_SEARCHPROGRESS 10042
#define IDC_SDOWNLOAD 10043
wxSizer *searchDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_CATEGORIES 10044
#define ID_DLOADLIST 10045
wxSizer *transferTopPane( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *queueSizer;
#define ID_QUEUELIST 10046
#define ID_UPLOADLIST 10047
wxSizer *transferBottomPane( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_FRIENDLIST 10048
#define IDC_CHATSELECTOR 10049
#define ID_HTMLWIN 10050
#define IDC_CMESSAGE 10051
#define IDC_CSEND 10052
#define IDC_CCLOSE 10053
wxSizer *messagePage( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *IDC_FD_X0;
#define IDC_FD_X1 10054
#define IDC_FNAME 10055
#define IDC_FD_X2 10056
#define IDC_METFILE 10057
#define IDC_FD_X3 10058
#define IDC_FHASH 10059
#define IDC_FD_X4 10060
#define IDC_FSIZE 10061
#define IDC_FD_X5 10062
#define IDC_PFSTATUS 10063
#define IDC_FD_X15 10064
#define IDC_LASTSEENCOMPL 10065
extern wxSizer *IDC_MINF_0;
#define IDC_MINF_1 10066
#define IDC_MINFCODEC 10067
#define IDC_MINF_2 10068
#define IDC_MINFBIT 10069
#define IDC_MINF_3 10070
#define IDC_MINFLEN 10071
extern wxSizer *IDC_FD_X6;
#define IDC_FD_X7 10072
#define IDC_SOURCECOUNT 10073
#define IDC_FD_X10 10074
#define IDC_FD_X9 10075
#define IDC_PARTCOUNT 10076
#define IDC_FD_X11 10077
#define IDC_FD_X13 10078
#define IDC_DATARATE 10079
#define IDC_FD_X14 10080
#define IDC_TRANSFERED 10081
#define IDC_FD_X12 10082
#define IDC_SOURCECOUNT2 10083
#define IDC_PARTAVAILABLE 10084
#define IDC_COMPLSIZE 10085
#define IDC_PROCCOMPL 10086
extern wxSizer *IDC_FD_ICH;
#define IDC_FD_LSTATS1 10087
#define IDC_FD_STATS1 10088
#define IDC_FD_LSTATS2 10089
#define IDC_FD_STATS2 10090
#define IDC_FD_LSTATS3 10091
#define IDC_FD_STATS3 10092
extern wxSizer *IDC_FD_SN;
#define IDC_LISTCTRLFILENAMES 10093
#define IDC_TAKEOVER 10094
#define IDC_BUTTONSTRIP 10095
#define IDC_FILENAME 10096
#define IDC_RENAME 10097
#define IDC_CMTBT 10098
#define ID_CLOSEWNDFD 5100
wxSizer *fileDetails( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_CMT_TEXT 10099
#define IDC_FC_CLEAR 10100
#define IDC_RATELIST 10101
#define IDCOK 10102
#define IDCCANCEL 10103
wxSizer *commentDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_LST 10104
#define IDC_CMSTATUS 10105
#define IDCREF 10106
wxSizer *commentLstDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_ANIMATE 10107
wxSizer *downloadDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_IPADDRESS 10108
#define ID_IPORT 10109
#define ID_USERNAME 10110
#define ID_USERHASH 10111
#define ID_ADDFRIEND 10112
#define ID_CLOSEDLG 10113
wxSizer *addFriendDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SHFILELIST 10114
#define IDC_SREQUESTED 10115
#define ID_GAUGE 10116
#define IDC_SREQUESTED2 10117
#define IDC_SACCEPTED 10118
#define IDC_SACCEPTED2 10119
#define IDC_STRANSFERED 10120
#define IDC_STRANSFERED2 10121
#define IDC_RELOADSHAREDFILES 10122
wxSizer *sharedfilesDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SEARCHLISTCTRL 10123
wxSizer *searchPage( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

extern wxSizer *testSizer;
#define ID_DLOADSCOPE 10124
#define IDC_C0 10125
#define IDC_C0_3 10126
#define IDC_C0_2 10127
#define ID_OTHERS 10128
#define IDC_S3 10129
#define IDC_S0 10130
#define ID_ACTIVEC 10131
#define IDC_S1 10132
#define ID_ULOADSCOPE 10133
#define IDC_C1 10134
#define IDC_C1_3 10135
#define IDC_C1_2 10136
#define ID_TREECTRL 10137
wxSizer *statsDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_DNAME 10138
#define ID_DHASH 10139
#define ID_DSOFT 10140
#define ID_DIP 10141
#define ID_DSIP 10142
#define ID_DVERSION 10143
#define ID_DID 10144
#define ID_DSNAME 10145
#define ID_DDOWNLOADING 10146
#define ID_DDOWN 10147
#define ID_DAVDR 10148
#define ID_DDOWNTOTAL 10149
#define ID_DDUP 10150
#define ID_DAVUR 10151
#define ID_DUPTOTAL 10152
#define ID_DRATIO 10153
#define ID_DRATING 10154
#define IDC_CDIDENT 10155
#define ID_DSCORE 10156
#define ID_CLOSEWND 10157
wxSizer *clientDetails( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_NICK 10158
#define IDC_LANGUAGE 10159
#define IDC_3DDEP 10160
#define IDC_3DDEPTH 10161
#define IDC_FLAT 10162
#define IDC_ROUND 10163
#define IDC_BEEPER 10164
#define IDC_BRINGTOFOREGROUND 10165
#define IDC_DBLCLICK 10166
#define IDC_MINTRAY 10167
#define IDC_ONLINESIG 10168
#define IDC_EXIT 10169
#define IDC_TOOLTIPDELAY_LBL 10170
#define IDC_TOOLTIPDELAY 10171
#define ID_DESKTOPMODE 10172
#define IDC_DAYS 10173
#define IDC_CHECK4UPDATE 10174
#define IDC_CHECKDAYS 10175
#define IDC_SPLASHON 10176
#define IDC_STARTMIN 10177
#define IDC_FED2KLH 10178
wxSizer *PreferencesGeneralTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_DCAP_LBL 10179
#define IDC_DOWNLOAD_CAP 10180
#define IDC_KBS2 10181
#define IDC_UCAP_LBL 10182
#define IDC_UPLOAD_CAP 10183
#define IDC_KBS3 10184
#define IDC_DLIMIT_LBL 10185
#define IDC_MAXDOWN 10186
#define IDC_KBS1 10187
#define IDC_ULIMIT_LBL 10188
#define IDC_MAXUP 10189
#define IDC_KBS4 10190
#define IDC_SLOTALLOC 10191
#define IDC_PORT 10192
#define IDC_UDPPORT 10193
#define IDC_UDPDISABLE 10194
#define IDC_MAXSRCHARD_LBL 10195
#define IDC_MAXSOURCEPERFILE 10196
#define IDC_MAXCONLABEL 10197
#define IDC_MAXCON 10198
#define IDC_AUTOCONNECT 10199
#define IDC_RECONN 10200
#define IDC_SHOWOVERHEAD 10201
#define IDC_WIZARD 10202
extern wxSizer *IDC_IPF_0;
#define IDC_IPFRELOAD 10203
#define IDC_IPFONOFF 10204
wxSizer *PreferencesConnectionTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_REMOVEDEAD 10205
#define IDC_SERVERRETRIES 10206
#define IDC_RETRIES_LBL 10207
#define IDC_AUTOSERVER 10208
#define IDC_EDITADR 10209
#define IDC_FILTER 10210
#define IDC_UPDATESERVERCONNECT 10211
#define IDC_UPDATESERVERCLIENT 10212
#define IDC_SCORE 10213
#define IDC_SMARTIDCHECK 10214
#define IDC_SAFESERVERCONNECT 10215
#define IDC_AUTOCONNECTSTATICONLY 10216
#define IDC_MANUALSERVERHIGHPRIO 10217
wxSizer *PreferencesServerTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_SEESHARE1 10218
#define IDC_SEESHARE2 10219
#define IDC_SEESHARE3 10220
#define IDC_ICH 10221
#define IDC_ADDNEWFILESPAUSED 10222
#define IDC_DAP 10223
#define IDC_PREVIEWPRIO 10224
#define IDC_UAP 10225
#define IDC_FULLCHUNKTRANS 10226
#define IDC_STARTNEXTFILE 10227
wxSizer *PreferencesFilesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_INCFILES 10228
#define IDC_SELINCDIR 10229
#define IDC_TEMPFILES 10230
#define IDC_SELTEMPDIR 10231
#define IDC_SHARESELECTOR 10232
#define IDC_VIDEOPLAYER 10233
#define IDC_BROWSEV 10234
#define IDC_VIDEOBACKUP 10235
wxSizer *PreferencesDirectoriesTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_SLIDERINFO 10236
#define IDC_SLIDER 10237
#define IDC_SLIDERINFO3 10238
#define IDC_SLIDER3 10239
#define IDC_PREFCOLORS 10240
#define IDC_COLORSELECTOR 10241
#define IDC_COLOR_BUTTON 10242
#define IDC_SLIDERINFO2 10243
#define IDC_SLIDER2 10244
wxSizer *PreferencesStatisticsTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_CB_TBN_USESOUND 10245
#define IDC_EDIT_TBN_WAVFILE 10246
#define IDC_BTN_BROWSE_WAV 10247
#define IDC_CB_TBN_ONLOG 10248
#define IDC_CB_TBN_ONCHAT 10249
#define IDC_CB_TBN_POP_ALWAYS 10250
#define IDC_CB_TBN_ONDOWNLOAD 10251
#define IDC_CB_TBN_ONNEWVERSION 10252
#define IDC_CB_TBN_IMPORTATNT 10253
#define IDC_SENDMAIL 10254
#define IDC_SMTP 10255
#define IDC_EMAIL 10256
wxSizer *PreferencesNotifyTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_WARNING 10257
#define IDC_STATIC 10258
#define IDC_MAXCON5SECLABEL 10259
#define IDC_MAXCON5SEC 10260
#define IDC_VERBOSE 10261
#define IDC_AUTOTAKEED2KLINKS 10262
#define IDC_SHOWRATEONTITLE 10263
#define IDC_FILEBUFFERSIZE_STATIC 10264
#define IDC_FILEBUFFERSIZE 10265
#define IDC_QUEUESIZE_STATIC 10266
#define IDC_QUEUESIZE 10267
#define IDC_SERVERKEEPALIVE_LABEL 10268
#define IDC_SERVERKEEPALIVE 10269
#define IDC_UPDATEQUEUE 10270
#define IDC_LISTREFRESH_LABEL 10271
#define IDC_LISTREFRESH 10272
wxSizer *PreferencesaMuleTweaksTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ENABLE_AUTO_ARRANGING 10273
#define ID_RADIOBUTTON 10274
#define IDC_SELECTED_THEME 10275
#define IDC_USE_FONT 10276
#define IDC_SELECTED_FONT 10277
#define IDC_FONT_BUTTON 10278
#define IDC_PREVIEW_THEME 10279
#define IDC_APPLY_THEME 10280
wxSizer *PreferencesGuiTweaksTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ENABLE_AUTO_NNS 10281
#define IDC_AUTO_NNS_EXTENDED_RADIO 10282
#define IDC_ENABLE_AUTO_FQS 10283
#define IDC_ENABLE_AUTO_HQRS 10284
#define IDC_HQR_VALUE 10285
#define IDC_AUTO_DROP_TIMER 10286
wxSizer *PreferencesSourcesDroppingTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_ENABLE_WEB 10287
#define IDC_WEB_PORT 10288
#define IDC_WEB_REFRESH_TIMEOUT 10289
#define IDC_WEB_GZIP 10290
#define IDC_ENABLE_WEB_LOW 10291
#define IDC_WEB_PASSWD 10292
#define IDC_WEB_PASSWD_LOW 10293
#define IDC_EXT_CONN_ACCEPT 10294
#define IDC_EXT_CONN_USETCP 10295
#define IDC_EXT_CONN_TCP_PORT 10296
#define IDC_EXT_CONN_PASSWD_ENABLE 10297
#define IDC_EXT_CONN_PASSWD 10298
wxSizer *PreferencesRemoteControlsTab( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NOTEBOOK_TOP 10299
#define ID_PREFS_OK_TOP 10300
#define ID_PREFS_CANCEL_TOP 10301
wxSizer *preferencesDlgTop( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define IDC_STATIC_TITLE 10302
#define IDC_TITLE 10303
#define IDC_STATIC_COMMENT 10304
#define IDC_COMMENT 10305
#define IDC_STATIC_INCOMING 10306
#define IDC_INCOMING 10307
#define IDC_BROWSE 10308
#define IDC_STATIC_PRIO 10309
#define IDC_PRIOCOMBO 10310
#define IDC_STATIC_COLOR 10311
#define ID_BOX_CATCOLOR 10312
#define IDC_CATCOLOR 10313
wxSizer *CategoriesEditWindow( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SPLATTER 10314
wxSizer *transferDlg( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SERVERINFO 10315
wxSizer *ServerInfoLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_LOGVIEW 10316
#define ID_BTN_RESET 10317
wxSizer *aMuleLog( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_UPDATELIST 10318
#define IDC_SERVERLISTURL 10319
#define IDC_SERVERNAME 10320
#define IDC_IPADDRESS 10321
#define IDC_SPORT 10322
#define ID_ADDTOLIST 10323
#define ID_SERVERLIST 10324
wxSizer *serverListDlgUp( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_SRVLOG_NOTEBOOK 10325
wxSizer *serverListDlgDown( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NOTEBOOK_LEFT 10326
#define ID_PREFS_OK_LEFT 10327
#define ID_PREFS_CANCEL_LEFT 10328
wxSizer *preferencesDlgLeft( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

// Declare menubar functions

// Declare toolbar functions

#define ID_BUTTONCONNECT 10329
#define ID_BUTTONSERVERS 10330
#define ID_BUTTONSEARCH 10331
#define ID_BUTTONTRANSFER 10332
#define ID_BUTTONSHARED 10333
#define ID_BUTTONMESSAGES 10334
#define ID_BUTTONSTATISTICS 10335
#define ID_BUTTONPREFERENCES 10336
#define ID_BUTTONNEWPREFERENCES 10337
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
