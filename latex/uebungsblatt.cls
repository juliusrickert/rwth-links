\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{uebungsblatt}

%% Custom package options
%% Fallback
%% Execute default options
%% Terminate option processing
\ProcessOptions\relax

%% Load base
\LoadClass[a4paper,12pt]{article}

%% Typographie
\usepackage{lmodern}
\usepackage[onehalfspacing]{setspace}

%% Layout
\RequirePackage{fancyhdr}
\usepackage{enumerate}

%% Packages für Source Code Einbindung
\RequirePackage{listings}
\RequirePackage{courier}
\RequirePackage{color}
\RequirePackage{caption}

%% Packages zur korrekten Darstellung der dt. Umlaute
\usepackage{ngerman}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}

%% Packages zur Einbindung von Mathematik
\usepackage{mathtools}
\usepackage{amsfonts}

%% Source Code Formatierung
\definecolor{name}{rgb}{0.5,0.5,0.5}
\definecolor{javared}{rgb}{0.6,0,0} % for strings
\definecolor{javagreen}{rgb}{0.25,0.5,0.35} % comments
\definecolor{javapurple}{rgb}{0.5,0,0.35} % keywords
\definecolor{javadocblue}{rgb}{0.25,0.35,0.75} % javadoc

\lstset{
  language=C,
  basicstyle=\footnotesize\ttfamily,
  numbers=left,
  numberstyle=\tiny,
  numbersep=10pt,
  tabsize=2,
  extendedchars=true,
  breaklines=true,
  keywordstyle=\color{javapurple}\bfseries,
  stringstyle=\color{javared},
  commentstyle=\color{javagreen},
  morecomment=[s][\color{javadocblue}]{/**}{*/},
  frame=b,
  showspaces=false,
  showtabs=false,
  xleftmargin=17pt,
  framexleftmargin=17pt,
  framexrightmargin=5pt,
  framexbottommargin=4pt,
  showstringspaces=false,
  inputencoding=utf8
}

\DeclareCaptionFont{white}{\color{white}}
\DeclareCaptionFormat{listing} {
  \colorbox[cmyk]{1,1,1,0}{
    \parbox{\textwidth}{#1#2#3}
  }
}
\captionsetup[lstlisting]{
  format=listing,
  labelfont=white,
  textfont=white,
  singlelinecheck=false,
  margin=0pt,
  font={bf,footnotesize}
}

%% Aufgaben Kommando
\setlength{\parindent}{0em}
\topmargin -1.0cm
\oddsidemargin 0cm
\evensidemargin 0cm
\setlength{\textheight}{9.2in}
\setlength{\textwidth}{6.0in}

\newcommand{\exercise}[1]{
  {
  \vspace*{1.5cm}
  \textsf{\textbf{Aufgabe #1}}
  \vspace*{0.2cm}\\
  }
}

%% Header
\newcommand{\header}[6]{
  \thispagestyle{fancy}
  \lhead{\sf \large #1 \\ \small #3 - #4}
  \rhead{\sf #2 \\  Übungsgruppe #5}
  \vspace*{0.2cm}
  \begin{center}
  \LARGE \sf \textbf{Übungsblatt #6}
  \end{center}
  \vspace*{0.2cm}
}
\endinput
