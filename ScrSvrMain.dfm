object Main: TMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsNone
  Caption = 'Main'
  ClientHeight = 338
  ClientWidth = 651
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object TimerDateTime: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerDateTimeTimer
    Left = 72
    Top = 48
  end
  object TimerPosition: TTimer
    Enabled = False
    OnTimer = TimerPositionTimer
    Left = 128
    Top = 48
  end
end
