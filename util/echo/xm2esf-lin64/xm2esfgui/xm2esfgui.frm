VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "XM2ESF/GUI v0.2 by Oerg866"
   ClientHeight    =   8055
   ClientLeft      =   150
   ClientTop       =   780
   ClientWidth     =   9015
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8055
   ScaleWidth      =   9015
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "XM File (Please use absolute paths unless you're REALLY SURE what you're doing...)"
      Height          =   855
      Left            =   120
      TabIndex        =   102
      Top             =   120
      Width           =   8775
      Begin VB.TextBox Text1 
         Height          =   285
         Left            =   120
         TabIndex        =   104
         Top             =   360
         Width           =   7455
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Browse..."
         Height          =   495
         Left            =   7680
         TabIndex        =   103
         Top             =   240
         Width           =   975
      End
   End
   Begin VB.Frame Frame8 
      Caption         =   "PSG Noise"
      Height          =   1935
      Left            =   5880
      TabIndex        =   23
      Top             =   4800
      Width           =   3015
      Begin VB.Frame Frame10 
         Caption         =   "Noise Type"
         Height          =   1575
         Left            =   120
         TabIndex        =   99
         Top             =   240
         Visible         =   0   'False
         Width           =   1215
         Begin VB.OptionButton noisetype 
            Caption         =   "White Noise"
            Height          =   375
            Index           =   0
            Left            =   120
            TabIndex        =   101
            Top             =   360
            Value           =   -1  'True
            Width           =   855
         End
         Begin VB.OptionButton noisetype 
            Caption         =   "Periodic Noise"
            Height          =   375
            Index           =   1
            Left            =   120
            TabIndex        =   100
            Top             =   960
            Width           =   975
         End
      End
      Begin VB.Frame Frame9 
         Caption         =   "Noise Frequency"
         Height          =   1575
         Left            =   1440
         TabIndex        =   96
         Top             =   240
         Visible         =   0   'False
         Width           =   1455
         Begin VB.OptionButton noisefreq 
            Caption         =   """High SMS Drum"""
            Height          =   435
            Index           =   0
            Left            =   120
            TabIndex        =   98
            Top             =   360
            Width           =   1215
         End
         Begin VB.OptionButton noisefreq 
            Caption         =   "Controlled by note pitch"
            Height          =   435
            Index           =   1
            Left            =   120
            TabIndex        =   97
            Top             =   960
            Width           =   1215
         End
      End
   End
   Begin VB.Frame Frame7 
      Caption         =   "Pitch control"
      Height          =   1575
      Left            =   2880
      TabIndex        =   22
      Top             =   6120
      Width           =   2895
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   9
         Left            =   2520
         Max             =   192
         TabIndex        =   92
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   8
         Left            =   2160
         Max             =   192
         TabIndex        =   91
         Top             =   480
         Value           =   60
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   7
         Left            =   1920
         Max             =   192
         TabIndex        =   90
         Top             =   480
         Value           =   60
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   6
         Left            =   1680
         Max             =   192
         TabIndex        =   89
         Top             =   480
         Value           =   60
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   5
         Left            =   1320
         Max             =   192
         TabIndex        =   88
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   4
         Left            =   1080
         Max             =   192
         TabIndex        =   87
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   3
         Left            =   840
         Max             =   192
         TabIndex        =   86
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   2
         Left            =   600
         Max             =   192
         TabIndex        =   85
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   1
         Left            =   360
         Max             =   192
         TabIndex        =   84
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.VScrollBar pitch 
         Height          =   495
         Index           =   0
         Left            =   120
         Max             =   192
         TabIndex        =   70
         Top             =   480
         Value           =   96
         Visible         =   0   'False
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   9
         Left            =   2520
         TabIndex        =   83
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "-36"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   8
         Left            =   2160
         TabIndex        =   82
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "-36"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   7
         Left            =   1920
         TabIndex        =   81
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "-36"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   6
         Left            =   1680
         TabIndex        =   80
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   5
         Left            =   1320
         TabIndex        =   79
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   4
         Left            =   1080
         TabIndex        =   78
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   3
         Left            =   840
         TabIndex        =   77
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   2
         Left            =   600
         TabIndex        =   76
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   1
         Left            =   360
         TabIndex        =   75
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label19 
         Alignment       =   2  'Center
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "Small Fonts"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   135
         Index           =   0
         Left            =   120
         TabIndex        =   74
         Top             =   1080
         Width           =   255
      End
      Begin VB.Label Label18 
         Caption         =   "Noise"
         Height          =   255
         Left            =   2400
         TabIndex        =   73
         Top             =   240
         Width           =   400
      End
      Begin VB.Label Label17 
         Caption         =   "PSG"
         Height          =   255
         Left            =   1680
         TabIndex        =   72
         Top             =   240
         Width           =   495
      End
      Begin VB.Label Label16 
         Caption         =   "FM"
         Height          =   255
         Left            =   120
         TabIndex        =   71
         Top             =   240
         Width           =   495
      End
   End
   Begin VB.Frame Frame6 
      Caption         =   "Channel volume"
      Height          =   3855
      Left            =   120
      TabIndex        =   21
      Top             =   3840
      Width           =   2655
      Begin MSComctlLib.Slider Slider2 
         Height          =   315
         Index           =   0
         Left            =   720
         TabIndex        =   40
         Top             =   2280
         Visible         =   0   'False
         Width           =   1455
         _ExtentX        =   2566
         _ExtentY        =   556
         _Version        =   393216
         Max             =   64
         SelStart        =   64
         Value           =   64
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   1335
         Index           =   0
         Left            =   120
         TabIndex        =   25
         Top             =   480
         Visible         =   0   'False
         Width           =   375
         _ExtentX        =   661
         _ExtentY        =   2355
         _Version        =   393216
         Orientation     =   1
         Max             =   64
         SelStart        =   48
         Value           =   48
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   1335
         Index           =   1
         Left            =   480
         TabIndex        =   27
         Top             =   480
         Visible         =   0   'False
         Width           =   375
         _ExtentX        =   661
         _ExtentY        =   2355
         _Version        =   393216
         Orientation     =   1
         Max             =   64
         SelStart        =   48
         Value           =   48
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   1335
         Index           =   2
         Left            =   840
         TabIndex        =   28
         Top             =   480
         Visible         =   0   'False
         Width           =   375
         _ExtentX        =   661
         _ExtentY        =   2355
         _Version        =   393216
         Orientation     =   1
         Max             =   64
         SelStart        =   48
         Value           =   48
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   1335
         Index           =   3
         Left            =   1200
         TabIndex        =   29
         Top             =   480
         Visible         =   0   'False
         Width           =   375
         _ExtentX        =   661
         _ExtentY        =   2355
         _Version        =   393216
         Orientation     =   1
         Max             =   64
         SelStart        =   48
         Value           =   48
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   1335
         Index           =   4
         Left            =   1560
         TabIndex        =   30
         Top             =   480
         Visible         =   0   'False
         Width           =   375
         _ExtentX        =   661
         _ExtentY        =   2355
         _Version        =   393216
         Orientation     =   1
         Max             =   64
         SelStart        =   48
         Value           =   48
      End
      Begin MSComctlLib.Slider Slider1 
         Height          =   1335
         Index           =   5
         Left            =   1920
         TabIndex        =   31
         Top             =   480
         Visible         =   0   'False
         Width           =   375
         _ExtentX        =   661
         _ExtentY        =   2355
         _Version        =   393216
         Orientation     =   1
         Max             =   64
         SelStart        =   48
         Value           =   48
      End
      Begin MSComctlLib.Slider Slider2 
         Height          =   315
         Index           =   1
         Left            =   720
         TabIndex        =   43
         Top             =   2640
         Visible         =   0   'False
         Width           =   1455
         _ExtentX        =   2566
         _ExtentY        =   556
         _Version        =   393216
         Enabled         =   0   'False
         Max             =   64
         SelStart        =   64
         Value           =   64
      End
      Begin MSComctlLib.Slider Slider2 
         Height          =   315
         Index           =   2
         Left            =   720
         TabIndex        =   44
         Top             =   3000
         Visible         =   0   'False
         Width           =   1455
         _ExtentX        =   2566
         _ExtentY        =   556
         _Version        =   393216
         Max             =   64
         SelStart        =   15
         Value           =   64
      End
      Begin MSComctlLib.Slider Slider2 
         Height          =   315
         Index           =   3
         Left            =   720
         TabIndex        =   93
         Top             =   3360
         Visible         =   0   'False
         Width           =   1455
         _ExtentX        =   2566
         _ExtentY        =   556
         _Version        =   393216
         Max             =   64
         SelStart        =   64
         Value           =   64
      End
      Begin VB.Label Label20 
         Caption         =   "Noise"
         Height          =   255
         Left            =   120
         TabIndex        =   95
         Top             =   3360
         Width           =   495
      End
      Begin VB.Label psgvol 
         Caption         =   "64"
         Height          =   255
         Index           =   3
         Left            =   2280
         TabIndex        =   94
         Top             =   3360
         Width           =   255
      End
      Begin VB.Label psgvol 
         Caption         =   "64"
         Height          =   255
         Index           =   2
         Left            =   2280
         TabIndex        =   47
         Top             =   3000
         Width           =   255
      End
      Begin VB.Label psgvol 
         Caption         =   "64"
         Height          =   255
         Index           =   1
         Left            =   2280
         TabIndex        =   46
         Top             =   2640
         Width           =   255
      End
      Begin VB.Label Label14 
         Alignment       =   2  'Center
         Caption         =   "0 = Loudest"
         Height          =   255
         Left            =   1200
         TabIndex        =   45
         Top             =   2040
         Width           =   975
      End
      Begin VB.Line Line4 
         X1              =   120
         X2              =   2520
         Y1              =   2160
         Y2              =   2160
      End
      Begin VB.Label Label13 
         Caption         =   "PSG"
         Height          =   375
         Left            =   120
         TabIndex        =   42
         Top             =   2280
         Width           =   375
      End
      Begin VB.Label psgvol 
         Caption         =   "64"
         Height          =   255
         Index           =   0
         Left            =   2280
         TabIndex        =   41
         Top             =   2280
         Width           =   255
      End
      Begin VB.Line Line3 
         X1              =   645
         X2              =   645
         Y1              =   2280
         Y2              =   3600
      End
      Begin VB.Label fmvol 
         Alignment       =   2  'Center
         Caption         =   "48"
         BeginProperty Font 
            Name            =   "Terminal"
            Size            =   6
            Charset         =   255
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   5
         Left            =   1920
         TabIndex        =   39
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label fmvol 
         Alignment       =   2  'Center
         Caption         =   "48"
         BeginProperty Font 
            Name            =   "Terminal"
            Size            =   6
            Charset         =   255
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   4
         Left            =   1560
         TabIndex        =   38
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label fmvol 
         Alignment       =   2  'Center
         Caption         =   "48"
         BeginProperty Font 
            Name            =   "Terminal"
            Size            =   6
            Charset         =   255
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   3
         Left            =   1200
         TabIndex        =   37
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label fmvol 
         Alignment       =   2  'Center
         Caption         =   "48"
         BeginProperty Font 
            Name            =   "Terminal"
            Size            =   6
            Charset         =   255
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   2
         Left            =   840
         TabIndex        =   36
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label fmvol 
         Alignment       =   2  'Center
         Caption         =   "48"
         BeginProperty Font 
            Name            =   "Terminal"
            Size            =   6
            Charset         =   255
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   1
         Left            =   480
         TabIndex        =   35
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label fmvol 
         Alignment       =   2  'Center
         Caption         =   "48"
         BeginProperty Font 
            Name            =   "Terminal"
            Size            =   6
            Charset         =   255
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   34
         Top             =   1920
         Width           =   255
      End
      Begin VB.Label Label11 
         Caption         =   "64"
         Height          =   255
         Left            =   2280
         TabIndex        =   33
         Top             =   1560
         Width           =   255
      End
      Begin VB.Label Label10 
         Caption         =   "0"
         Height          =   255
         Left            =   2400
         TabIndex        =   32
         Top             =   600
         Width           =   135
      End
      Begin VB.Label Label9 
         Alignment       =   2  'Center
         Caption         =   " 0 = Loudest "
         Height          =   255
         Left            =   1200
         TabIndex        =   26
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label Label8 
         Alignment       =   2  'Center
         Caption         =   " FM "
         Height          =   255
         Left            =   360
         TabIndex        =   24
         Top             =   240
         Width           =   375
      End
      Begin VB.Line Line2 
         X1              =   2520
         X2              =   120
         Y1              =   315
         Y2              =   315
      End
   End
   Begin VB.Frame Frame5 
      Caption         =   "Channel assignments"
      Height          =   4935
      Left            =   2880
      TabIndex        =   12
      Top             =   1080
      Width           =   2895
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   10
         Left            =   1560
         TabIndex        =   69
         Top             =   4440
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   9
         Left            =   1560
         TabIndex        =   68
         Top             =   3840
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   8
         Left            =   1560
         TabIndex        =   67
         Top             =   3480
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   7
         Left            =   1560
         TabIndex        =   66
         Top             =   3120
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   6
         Left            =   1560
         TabIndex        =   65
         Top             =   2760
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   5
         Left            =   1560
         TabIndex        =   64
         Top             =   2160
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   4
         Left            =   1560
         TabIndex        =   63
         Top             =   1800
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   3
         Left            =   1560
         TabIndex        =   62
         Top             =   1440
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   2
         Left            =   1560
         TabIndex        =   61
         Top             =   1080
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   1
         Left            =   1560
         TabIndex        =   60
         Top             =   720
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.ComboBox xmchan 
         Height          =   315
         Index           =   0
         Left            =   1560
         TabIndex        =   59
         Top             =   360
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "PCM"
         Height          =   255
         Index           =   10
         Left            =   240
         TabIndex        =   58
         Top             =   4440
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "Noise"
         Height          =   255
         Index           =   9
         Left            =   240
         TabIndex        =   57
         Top             =   3840
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "PSG3"
         Height          =   255
         Index           =   8
         Left            =   240
         TabIndex        =   56
         Top             =   3480
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "PSG2"
         Height          =   255
         Index           =   7
         Left            =   240
         TabIndex        =   55
         Top             =   3120
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "PSG1"
         Height          =   255
         Index           =   6
         Left            =   240
         TabIndex        =   54
         Top             =   2760
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "FM6"
         Height          =   255
         Index           =   5
         Left            =   240
         TabIndex        =   53
         Top             =   2160
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "FM5"
         Height          =   255
         Index           =   4
         Left            =   240
         TabIndex        =   52
         Top             =   1800
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "FM4"
         Height          =   255
         Index           =   3
         Left            =   240
         TabIndex        =   51
         Top             =   1440
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "FM3"
         Height          =   255
         Index           =   2
         Left            =   240
         TabIndex        =   50
         Top             =   1080
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "FM2"
         Height          =   255
         Index           =   1
         Left            =   240
         TabIndex        =   49
         Top             =   720
         Visible         =   0   'False
         Width           =   855
      End
      Begin VB.Label chan 
         Alignment       =   1  'Right Justify
         BorderStyle     =   1  'Fixed Single
         Caption         =   "FM1"
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   48
         Top             =   360
         Visible         =   0   'False
         Width           =   855
      End
   End
   Begin VB.Frame Frame4 
      Caption         =   "Channels"
      Height          =   1455
      Left            =   120
      TabIndex        =   11
      Top             =   2280
      Width           =   2655
      Begin VB.CheckBox noise 
         Caption         =   "Noise"
         Height          =   375
         Left            =   1800
         TabIndex        =   18
         Top             =   840
         Width           =   735
      End
      Begin VB.VScrollBar psg 
         Height          =   735
         Left            =   960
         Max             =   3
         TabIndex        =   16
         Top             =   360
         Width           =   255
      End
      Begin VB.CheckBox pcm 
         Caption         =   "PCM"
         Height          =   255
         Left            =   1800
         TabIndex        =   14
         Top             =   360
         Width           =   735
      End
      Begin VB.VScrollBar fm 
         Height          =   735
         Left            =   120
         Max             =   6
         TabIndex        =   13
         Top             =   360
         Width           =   255
      End
      Begin VB.Label Label7 
         Caption         =   "0"
         Height          =   375
         Left            =   1320
         TabIndex        =   20
         Top             =   720
         Width           =   255
      End
      Begin VB.Label Label6 
         Caption         =   "0"
         Height          =   255
         Left            =   480
         TabIndex        =   19
         Top             =   720
         Width           =   255
      End
      Begin VB.Line Line1 
         Index           =   1
         X1              =   1725
         X2              =   1725
         Y1              =   360
         Y2              =   1200
      End
      Begin VB.Line Line1 
         Index           =   0
         X1              =   840
         X2              =   840
         Y1              =   360
         Y2              =   1200
      End
      Begin VB.Label Label5 
         Caption         =   "PSG"
         Height          =   255
         Left            =   1320
         TabIndex        =   17
         Top             =   360
         Width           =   375
      End
      Begin VB.Label Label4 
         Caption         =   "FM"
         Height          =   255
         Left            =   480
         TabIndex        =   15
         Top             =   360
         Width           =   375
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "Song properties"
      Height          =   1095
      Left            =   120
      TabIndex        =   7
      Top             =   1080
      Width           =   2655
      Begin MSComctlLib.Slider speed 
         Height          =   255
         Left            =   960
         TabIndex        =   106
         Top             =   720
         Width           =   1575
         _ExtentX        =   2778
         _ExtentY        =   450
         _Version        =   393216
         Min             =   1
         Max             =   32
         SelStart        =   7
         Value           =   7
      End
      Begin VB.OptionButton Option2 
         Caption         =   "BGM"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   10
         Top             =   240
         Value           =   -1  'True
         Width           =   975
      End
      Begin VB.CheckBox Check1 
         Caption         =   "Loop"
         Height          =   255
         Left            =   1800
         TabIndex        =   9
         Top             =   240
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.OptionButton Option2 
         Caption         =   "SFX"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   8
         Top             =   480
         Width           =   735
      End
      Begin VB.Label Label15 
         Alignment       =   2  'Center
         Caption         =   "7"
         Height          =   255
         Left            =   2280
         TabIndex        =   107
         Top             =   480
         Width           =   255
      End
      Begin VB.Label Label12 
         Caption         =   "Ticks per row:"
         Height          =   255
         Left            =   1080
         TabIndex        =   105
         Top             =   480
         Width           =   1095
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Instrument assignments"
      Height          =   3615
      Left            =   5880
      TabIndex        =   1
      Top             =   1080
      Width           =   3015
      Begin VB.ListBox List2 
         Height          =   2790
         Left            =   1680
         TabIndex        =   5
         Top             =   600
         Width           =   735
      End
      Begin VB.ListBox List1 
         Height          =   2790
         Left            =   600
         TabIndex        =   4
         Top             =   600
         Width           =   735
      End
      Begin VB.Label Label3 
         Alignment       =   2  'Center
         Caption         =   "->"
         Height          =   255
         Left            =   1320
         TabIndex        =   6
         Top             =   720
         Width           =   375
      End
      Begin VB.Label Label2 
         Caption         =   "Echo Instrument"
         Height          =   495
         Left            =   1680
         TabIndex        =   3
         Top             =   360
         Width           =   1215
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Caption         =   "XM Instrument"
         Height          =   375
         Left            =   240
         TabIndex        =   2
         Top             =   360
         Width           =   1095
      End
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Create XIF File"
      Height          =   855
      Left            =   5880
      TabIndex        =   0
      Top             =   6840
      Width           =   3015
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   8880
      Top             =   4200
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      Filter          =   "64"
   End
   Begin VB.Label Label21 
      Alignment       =   2  'Center
      Caption         =   $"xm2esfgui.frx":0000
      Height          =   255
      Left            =   120
      TabIndex        =   108
      Top             =   7800
      Width           =   8775
   End
   Begin VB.Menu fmenu 
      Caption         =   "&File"
      Begin VB.Menu openxif 
         Caption         =   "&Open XIF"
         Shortcut        =   ^O
      End
      Begin VB.Menu savexif 
         Caption         =   "&Save XIF"
         Shortcut        =   ^S
      End
      Begin VB.Menu exit 
         Caption         =   "&Exit"
         Shortcut        =   ^X
      End
   End
   Begin VB.Menu hmenu 
      Caption         =   "&Help"
      Begin VB.Menu about 
         Caption         =   "&About"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim saved As Boolean



Function param(strn$, b&) As String
    c& = 0
    i& = 1
    car$ = Mid$(strn$, 1, 1)
    If car$ = Chr$(34) Then
        comp$ = Chr$(34)
        i& = 2
        car$ = Mid$(strn$, 2, 1)
    Else
        comp$ = " "
    End If
    While c& <> b&
    While car$ <> comp$
          SPEFT$ = SPEFT$ + car$
          i& = i& + 1
          car$ = Mid$(strn$, i&, 1)
          If i& > Len(strn$) Then
              SPEFT$ = ""
              param = SPEFT$
              Exit Function
          End If
    Wend

   i& = i& + 1
   c& = c& + 1
   ' SPEFT$ = MID$(strn$, i&, 1)
    car$ = SPEFT$
    Wend


    param = RTrim$(SPLEFT(Mid$(strn$, i&)))

End Function

Function SPLEFT(strn$) As String

    i& = 1
    car$ = Mid$(strn$, 1, 1)
    If car$ = Chr$(34) Then
        comp$ = Chr$(34)
        i& = 2
        car$ = Mid$(strn$, 2, 1)
    Else
        comp$ = " "
    End If
    While car$ <> comp$
          SPEFT$ = SPEFT$ + car$
          i& = i& + 1
          car$ = Mid$(strn$, i&, 1)
          If i& > Len(strn$) Then
              SPLEFT = SPEFT$ + cars$
              Exit Function
          End If
    Wend
              SPLEFT = SPEFT$
End Function

Private Sub about_Click()
MsgBox "xm2esfgui / XIF creator v0.2" + vbNewLine + vbNewLine + "by Oerg866", , "About xm2esfgui..."
End Sub

Private Sub Check1_Click()
saved = False

End Sub

Private Sub Command1_Click()
If Text1.Text = "" Then
    MsgBox "You didn't select an XM file :(", , "Fatal"
    Exit Sub
End If

CommonDialog1.Filter = "XM Information File (*.xif) | *.xif|All Files (*.*)|*.*"

CommonDialog1.Flags = cdlOFNOverwritePrompt Or cdlOFNHideReadOnly
CommonDialog1.ShowSave

a$ = CommonDialog1.FileName

Open a$ For Output As #1
Print #1, "# Created by XM2ESFGUI"
Print #1, "# FROM DAT OERG866 YEYEYE"
Print #1, "FILE " + Chr$(34) + Text1.Text + Chr$(34)
temp$ = "TYPE "
If Option2(1).Value = True Then
    temp$ = temp$ + "SFX NOLOOP"
Else
    If Check1.Value = 1 Then
    temp$ = temp$ + "BGM LOOP"
    Else
    temp$ = temp$ + "BGM NOLOOP"
    End If
End If
Print #1, temp$
Print #1, "TEMPO" + Str$(speed.Value)
Print #1, "PSG " + Trim$(Str$(psg.Value))
Print #1, "NOISE " + Trim$(Str$(noise.Value))
Print #1, "FM " + Trim$(Str$(fm.Value))
Print #1, "PCM " + Trim$(Str$(pcm.Value))

For i = 1 To fm.Value
    Print #1, "FM" + Trim$(Str$(i)) + " " + Trim$(xmchan(i - 1).Text)
Next i

If pcm.Value = 1 Then
    Print #1, "PCMC " + Trim$(xmchan(10).Text)
End If

For i = 7 To psg.Value + 6
    Print #1, "PSG" + Trim$(Str$(i - 6)) + " " + Trim$(xmchan(i - 1).Text)
Next i

If noise.Value = 1 Then
    Print #1, "PSGN " + Trim$(xmchan(9).Text)
End If


If noise.Value = True Then
    temp$ = "NOISEFREQ"
    
    If noisefreq(1).Value Then
        temp$ = temp$ + " 0"
    Else
        temp$ = temp$ + " 1"
    
    End If
    
    Print #1, temp$
    
    temp$ = "NOISETYPE "
    
    If noisetype(0).Value Then
        temp$ = temp$ + " 1"
    Else
        temp$ = temp$ + " 0"
    
    End If
    
End If


Print #1, "[Instruments]"
For i = 1 To 128

    Print #1, Trim$(Str$(i)) + " " + Hex$(List1.ItemData(i - 1))

Next i

Print #1, "[Pitch]"

For i = 1 To fm.Value 'label19
    Print #1, "FM" + Trim$(Str$(i)) + " " + Trim$(Label19(i - 1).Caption)
Next i

For i = 1 To psg.Value
    Print #1, "PSG" + Trim$(Str$(i)) + " " + Trim$(Label19(i + 5).Caption)
Next i

If noise.Value = 1 Then
    Print #1, "PSGN " + Trim$(Str$(Label19(9).Caption))
End If

Print #1, "[Volume]"

For i = 1 To fm.Value 'label19
    Print #1, "FM" + Trim$(Str$(i)) + " " + Trim$(Str$(Slider1(i - 1).Value))
Next i

For i = 1 To psg.Value
    Print #1, "PSG" + Trim$(Str$(i)) + " " + Trim$(Str$(Slider2(i - 1).Value))
Next i

If noise.Value = 1 Then
    Print #1, "PSGN " + Trim$(Str$(Slider2(3).Value))
End If

Print #1, "[END]"
Close
saved = True

End Sub

Private Sub Command2_Click()
saved = False
CommonDialog1.Filter = "eXtended Module (*.xm) | *.xm|All Files (*.*)|*.*"
CommonDialog1.InitDir = App.Path


CommonDialog1.Flags = cdlOFNFileMustExist Or cdlOFNHideReadOnly

CommonDialog1.ShowOpen
a$ = CommonDialog1.FileName


If a$ = "" Then
MsgBox "No valid filename specified!", , "Error"
Else
Text1.Text = a$
End If

End Sub

Private Sub Option1_Click()

End Sub

Private Sub exit_Click()
If saved = False Then
a = MsgBox("Do you want to save this XIF?", vbYesNoCancel, "Unsaved content")
If a = vbYes Then
    Command1_Click
ElseIf a = vbNo Then
    saved = True
    
ElseIf a = vbCancel Then
    Exit Sub
End If
End If
If saved = False Then
    MsgBox "Error saving file!"
    Exit Sub
End If

Unload Me
End Sub

Private Sub fm_Change()

saved = False
If fm.Value = 6 And pcm.Value = 1 Then
MsgBox "Errm.... You can't put PCM and FM6 at the same time.", , "Error"
fm.Value = 5
Else
Label6.Caption = fm.Value
For i = 1 To fm.Value
Slider1(i - 1).Enabled = True
Slider1(i - 1).Visible = True
chan(i - 1).Visible = True
xmchan(i - 1).Visible = True
pitch(i - 1).Visible = True

Next i
For i = 1 To (6 - fm.Value)
Slider1(6 - i).Enabled = False
Slider1(6 - i).Visible = False
chan(6 - i).Visible = False
xmchan(6 - i).Visible = False
pitch(6 - i).Visible = False
Next i
End If
End Sub

Private Sub Form_Load()
    For i = 0 To 255
        If i < 32 Then
            For p = 0 To 10
                xmchan(p).AddItem Trim$(Str$(i + 1))
            Next p
        End If
        If i < 128 Then
            List1.AddItem Str$(i + 1)
        End If
        
        List2.AddItem String$(2 - Len(Hex$(i)), "0") + Hex$(i)
    Next i
    
End Sub

Private Sub List1_Click()
List2.Selected(List1.ItemData(List1.ListIndex)) = True
saved = False
End Sub

Private Sub List2_Click()
saved = False
If List1.ListIndex > -1 Then
List1.ItemData(List1.ListIndex) = List2.ListIndex
End If
End Sub

Private Sub noise_Click()
saved = False
If noise.Value = 1 Then
    Frame10.Visible = True
    Frame9.Visible = True
    chan(9).Visible = True
    xmchan(9).Visible = True
    pitch(9).Visible = True
    Slider2(3).Visible = True
Else
    Frame10.Visible = False
    Frame9.Visible = False
    chan(9).Visible = False
    xmchan(9).Visible = False
    pitch(9).Visible = False
    Slider2(3).Visible = False
End If
If noise.Value = 1 And psg.Value = 3 Then
    noisefreq(0).Value = True
    noisefreq(1).Enabled = False
    
Else
    noisefreq(1).Value = True
    noisefreq(1).Enabled = True
End If

End Sub

Private Sub noisefreq_Click(Index As Integer)
saved = False

End Sub

Private Sub noisetype_Click(Index As Integer)
saved = False

End Sub

Private Sub openxif_Click()
If saved = False Then
a = MsgBox("Do you want to save this XIF?", vbYesNoCancel, "Unsaved content")
If a = vbYes Then
    Command1_Click
ElseIf a = vbNo Then
    saved = True
    
ElseIf a = vbCancel Then
    Exit Sub
End If
End If
If saved = False Then
    Exit Sub 'Error during save
End If
CommonDialog1.Filter = "XM Information File (*.xif) | *.xif|All Files (*.*)|*.*"

CommonDialog1.Flags = cdlOFNFileMustExist Or cdlOFNHideReadOnly

CommonDialog1.ShowOpen

m$ = CommonDialog1.FileName

Open m$ For Input As #1

    While LCase$(setting$) <> "[instruments]"
        If Mid$(setting$, 1, 1) <> "#" Then
           Select Case SPLEFT(setting$)

              Case "FILE"
                Text1.Text = param(setting$, 1)
              Case "TYPE"
                Select Case param(setting$, 1)
                Case "BGM"
                Option2(0).Value = True
                Case "SFX"
                Option2(1).Value = True
                End Select
                Select Case param(setting$, 2)
                Case "LOOP"

                Check1.Value = 1
                Case "NOLOOP"
                
                Check1.Value = 0


                End Select

              Case "TEMPO"
                  speed.Value = Val(param(setting$, 1))
              Case "FM"
                  fm.Value = Val(param(setting$, 1))
                  
              Case "PSG"
                  psg.Value = Val(param(setting$, 1))
                  
              Case "PCM"
                  pcm.Value = 1
              Case "NOISE"
                  noise.Value = Val(param(setting$, 1))
              Case "FM1"
                  xmchan(0).ListIndex = Val(param(setting$, 1)) - 1
              Case "FM2"
                  xmchan(1).ListIndex = Val(param(setting$, 1)) - 1
              Case "FM3"
                  xmchan(2).ListIndex = Val(param(setting$, 1)) - 1
              Case "FM4"
                  xmchan(3).ListIndex = Val(param(setting$, 1)) - 1
              Case "FM5"
                  xmchan(4).ListIndex = Val(param(setting$, 1)) - 1
              Case "FM6"
                  xmchan(5).ListIndex = Val(param(setting$, 1)) - 1
              Case "PCMC"
                  xmchan(10).ListIndex = Val(param(setting$, 1)) - 1
              Case "PSG1"
                  xmchan(6).ListIndex = Val(param(setting$, 1)) - 1
              Case "PSG2"
                  xmchan(7).ListIndex = Val(param(setting$, 1)) - 1
              Case "PSG3"
                  xmchan(8).ListIndex = Val(param(setting$, 1)) - 1
              Case "PSGN"
                  xmchan(9).ListIndex = Val(param(setting$, 1)) - 1
              Case "NOISEFREQ" ' 1 = stock, 0 = psg3
                  noisefreq(0).Value = Val(param(setting$, 1))
                  noisefreq(1).Value = Not Val(param(setting$, 1))
              Case "NOISETYPE"
                  noisetype(0).Value = Val(param(setting$, 1))
                  noisetype(1).Value = Not Val(param(setting$, 1))
                  '1 = white noise, 0 = periodic noise
           End Select
        End If
           Line Input #1, setting$

    Wend

    'INSTRUMENT ASSIGNMENTS
            Line Input #1, setting$

    While LCase$(setting$) <> "[pitch]"

        If Mid$(setting$, 1, 1) <> "#" Then

        List1.ItemData(Val(SPLEFT(setting$)) - 1) = Val("&H" + param(setting$, 1))
        End If
            Line Input #1, setting$
        
    Wend
                               
    
    ' Internal channel to variable assignment help

    ' 123456 = FM
    ' 789    = PSG
    ' 11     = PCM
    ' 10     = NSE

            Line Input #1, setting$
                               

    While LCase$(setting$) <> "[volume]"
        If Mid$(setting$, 1, 1) <> "#" Then

            Select Case SPLEFT(setting$)
                Case "FM1"
                    pitch(0) = Val(param(setting$, 1)) + 96
                Case "FM2"
                    pitch(1) = Val(param(setting$, 1)) + 96
                Case "FM3"
                    pitch(2) = Val(param(setting$, 1)) + 96
                Case "FM4"
                    pitch(3) = Val(param(setting$, 1)) + 96
                Case "FM5"
                    pitch(4) = Val(param(setting$, 1)) + 96
                Case "FM6"
                    pitch(5) = Val(param(setting$, 1)) + 96

                Case "PSG1"
                    pitch(6) = Val(param(setting$, 1)) + 96
                Case "PSG2"
                    pitch(7) = Val(param(setting$, 1)) + 96
                Case "PSG3"
                    pitch(8) = Val(param(setting$, 1)) + 96
                Case "PSGN"
                    pitch(9) = Val(param(setting$, 1)) + 96
                End Select
        End If
        Line Input #1, setting$


    Wend
            Line Input #1, setting$



    While LCase$(setting$) <> "[end]"
        If Mid$(setting$, 1, 1) <> "#" Then

            Select Case SPLEFT(setting$)
                Case "FM1"
                    Slider1(0).Value = Val(param(setting$, 1))
                Case "FM2"
                    Slider1(1).Value = Val(param(setting$, 1))
                Case "FM3"
                    Slider1(2).Value = Val(param(setting$, 1))
                Case "FM4"
                    Slider1(3).Value = Val(param(setting$, 1))
                Case "FM5"
                    Slider1(4).Value = Val(param(setting$, 1))
                Case "FM6"
                    Slider1(5).Value = Val(param(setting$, 1))

                Case "PSG1"
                    Slider2(0).Value = Val(param(setting$, 1))
                Case "PSG2"
                    Slider2(1).Value = Val(param(setting$, 1))
                Case "PSG3"
                    Slider2(2).Value = Val(param(setting$, 1))
                Case "PSGN"
                    Slider2(3).Value = Val(param(setting$, 1))
                End Select
        End If
        Line Input #1, setting$


    Wend
    '
    Close
Close

End Sub

Private Sub Option2_Click(Index As Integer)
saved = False
If Index = 1 Then
    If Option2(Index).Value = True Then
        Check1.Value = 0
        Check1.Enabled = False
    End If
Else
    If Option2(Index).Value = True Then
        Check1.Value = 1
        Check1.Enabled = True
    End If
End If
End Sub

Private Sub VScroll1_Change()


End Sub

Private Sub pcm_Click()
saved = False
If pcm.Value = 1 And fm.Value = 6 Then
MsgBox "Errm.... You can't put PCM and FM6 at the same time.", , "Error"
pcm.Value = 0
End If
If pcm.Value = 1 Then

    chan(10).Visible = True
    xmchan(10).Visible = True
Else

    chan(10).Visible = False
    xmchan(10).Visible = False
End If

End Sub

Private Sub pitch_Change(Index As Integer)
saved = False
Label19(Index).Caption = pitch(Index).Value - 96

End Sub

Private Sub psg_Change()
saved = False
Label7.Caption = psg.Value
For i = 1 To psg.Value
Slider2(i - 1).Enabled = True
Slider2(i - 1).Visible = True
chan(i - 1 + 6).Visible = True
xmchan(i - 1 + 6).Visible = True
pitch(i - 1 + 6).Visible = True
Next i
For i = 1 To (3 - psg.Value)
Slider2(3 - i).Enabled = False
Slider2(3 - i).Visible = False
chan(3 - i + 6).Visible = False
xmchan(3 - i + 6).Visible = False
pitch(3 - i + 6).Visible = False
Next i
If psg.Value = 3 Then
    noisefreq(0).Value = True
    noisefreq(1).Enabled = False
Else
    noisefreq(1).Value = True
    noisefreq(1).Enabled = True
End If

End Sub

Private Sub savexif_Click()
Command1_Click
End Sub

Private Sub Slider1_Change(Index As Integer)
saved = False
fmvol(Index).Caption = Slider1(Index).Value
End Sub

Private Sub Slider1_Scroll(Index As Integer)
saved = False
fmvol(Index).Caption = Slider1(Index).Value
End Sub

Private Sub Slider2_Change(Index As Integer)
saved = False
psgvol(Index).Caption = Slider2(Index).Value
End Sub

Private Sub Slider2_Scroll(Index As Integer)
saved = False
psgvol(Index).Caption = Slider2(Index).Value
End Sub

Private Sub Slider3_Change()

End Sub

Private Sub Slider3_Click()

End Sub

Private Sub speed_Change()
saved = False
Label15.Caption = speed.Value
End Sub

Private Sub speed_Scroll()

Label15.Caption = speed.Value
End Sub

Private Sub Text1_Change()
saved = False

End Sub

Private Sub xmchan_Change(Index As Integer)
saved = False

End Sub
