Name:       ktimeclock
Version:    4.0.3
Release:    0%{dist}
License:    Artistic
Group:      Office
URL:        http://www.howlingfrog.com/opensource/ktimeclock/
Source:     %{name}-%{version}.tar.gz
BuildRoot:  /var/tmp/%{name}-%{version}
Packager:   graham@howlingfrog.com

BuildRequires:  cmake >= 2.4.6
BuildRequires:  kdelibs-devel
BuildRequires:  gcc-c++

Summary:    Task-based timeclock for KDE.
%description
KTimeclock is designed to be a personal timeclock to keep track of how much
time is spent working on various projects and the tasks that they involve.

%prep
rm -rf $RPM_BUILD_ROOT

%setup

%build
%cmake .
make VERBOSE=1 %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%docdir %{_prefix}/share/doc/HTML/en/ktimeclock
%doc %{_prefix}/share/doc/HTML/en/ktimeclock/*
%dir %{_prefix}/share/kde4/apps/ktimeclock
%{_prefix}/bin/ktimeclock
%{_prefix}/share/kde4/apps/ktimeclock/pics/*
%{_prefix}/share/kde4/apps/ktimeclock/ktimeclockui.rc
%{_prefix}/share/applications/kde4/ktimeclock.desktop
%{_prefix}/share/icons/hicolor/*/apps/ktimeclock.png
%{_prefix}/share/icons/hicolor/*/apps/ktimeclock.svgz

%changelog
* Wed Feb 15 2012 Graham TerMarsch (graham@howlingfrog.com) - 4.0.3-0
- Fix problem with lost time, when the timer kicks in at <1000ms and Qt tells
  us that the time in seconds since the last tick was "0s".

* Wed Feb 1 2012 Graham TerMarsch (graham@howlingfrog.com) - 4.0.2-0
- Don't alert on lost ticks, just add them in like we never missed them.

* Wed Jan 11 2012 Graham TerMarsch (graham@howlingfrog.com) - 4.0.1-0
- Double-click handler

* Tue Nov 9 2010 Graham TerMarsch (graham@howlingfrog.com) - 4.0.0-2
- Patch release, with several compilation updates

* Sat Nov 7 2010 Graham TerMarsch (graham@howlingfrog.com) - 4.0.0-1
- First update for ktimeclock-4.0.0

* Wed May 8 2002 Graham TerMarsch (graham@howlingfrog.com)
- Updated for KTimeclock-2.0.2

* Tue Jan 29 2002 Graham TerMarsch (graham@howlingfrog.com)
- Updated for KTimeclock-2.0.1
- Updated the SPEC file so that it could be used to build RPMs on more
  platforms than just Mandrake-8.1.

* Sat Nov 04 2001 Graham TerMarsch (graham@howlingfrog.com)
- Updated for Mandrake-8.1 and KDE-2.2.1

* Wed Jan 17 2001 Graham TerMarsch (graham@howlingfrog.com)
- Initial rewrite/release of KTimeclock-2.0
