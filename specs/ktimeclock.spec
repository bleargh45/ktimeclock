###############################################################################
# KTimeclock SPEC file
###############################################################################
# $Revision: 1.3 $
###############################################################################
# This SPEC file is known to work for compiling/building KTimeclock on all of
# the following platforms:
# - Mandrake 8.1
# - RedHat 7.2
#
# If you manage to build KTimeclock on a platform not listed above, please
# contact the author to let him know what (if any) changes were needed to be
# made to this SPEC file.
###############################################################################

Name:       ktimeclock
Version:    2.0.2
Release:    1
Copyright:  Artistic
Group:      Office
URL:        http://www.howlingfrog.com/opensource/ktimeclock/
Source:     http://www.howlingfrog.com/opensource/%{name}/downloads/v%{version}/tarball/%{name}-%{version}.tar.gz
BuildRoot:  /var/tmp/%{name}-%{version}
Packager:   graham@howlingfrog.com

Summary:    Task-based timeclock for KDE.
%description
KTimeclock is designed to be a personal timeclock to keep track of how much
time is spent working on various projects and the tasks that they involve.

%prep
rm -rf $RPM_BUILD_ROOT

%setup
CXXFLAGS="$RPM_OPT_FLAGS -DNDEBUG" \
    ./configure --prefix=%{_prefix}

%build
make

%install
make install DESTDIR="$RPM_BUILD_ROOT"

%clean
rm -rf $RPM_BUILD_ROOT

%files
%docdir %{_prefix}/share/doc/HTML/en/ktimeclock
%doc %{_prefix}/share/doc/HTML/en/ktimeclock/*
%dir %{_prefix}/share/apps/ktimeclock
%dir %{_prefix}/share/apps/ktimeclock/pics
%{_prefix}/bin/ktimeclock
%{_prefix}/share/apps/ktimeclock/pics/dockicon0.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon1.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon2.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon3.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon4.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon5.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon6.xpm
%{_prefix}/share/apps/ktimeclock/pics/dockicon7.xpm
%{_prefix}/share/apps/ktimeclock/ktimeclockui.rc
%{_prefix}/share/applnk/Utilities/ktimeclock.desktop
%{_prefix}/share/icons/hicolor/22x22/apps/ktimeclock.png
%{_prefix}/share/icons/hicolor/32x32/apps/ktimeclock.png
%{_prefix}/share/icons/hicolor/48x48/apps/ktimeclock.png
%{_prefix}/share/icons/locolor/16x16/apps/ktimeclock.png
%{_prefix}/share/icons/locolor/32x32/apps/ktimeclock.png

%changelog
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
