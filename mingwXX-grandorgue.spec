# GrandOrgue - free pipe organ simulator
# 
# Copyright 2006 Milan Digital Audio LLC
# Copyright 2009-2015 GrandOrgue contributors (see AUTHORS)
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

%if "X%{?_mingw_bitsize}" == "X"
# Set bitsize either to 32 or 64
%define _mingw_bitsize XX
%endif
%if "X%{?with_asio}" == "X"
# Enable building with ASIO
%define with_asio 0
%endif

%if %{_mingw_bitsize} == 32
%define __strip %{_mingw32_strip}
%define __objdump %{_mingw32_objdump}
%define _use_internal_dependency_generator 0
%define __find_requires %{_mingw32_findrequires}
%define __find_provides %{_mingw32_findprovides}
%define __os_install_post %{_mingw32_install_post}
%define _mingw_cmake %{_mingw32_cmake}
%define _mingw_bindir %{_mingw32_bindir}
%define _mingw_datadir %{_mingw32_datadir}
%else
%define __strip %{_mingw64_strip}
%define __objdump %{_mingw64_objdump}
%define _use_internal_dependency_generator 0
%define __find_requires %{_mingw64_findrequires}
%define __find_provides %{_mingw64_findprovides}
%define __os_install_post %{_mingw64_install_post}
%define _mingw_cmake %{_mingw64_cmake}
%define _mingw_bindir %{_mingw64_bindir}
%define _mingw_datadir %{_mingw64_datadir}
%endif

Summary:        Virtual Pipe Organ Software
Name:           mingw%{_mingw_bitsize}-grandorgue
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  gettext-tools
BuildRequires:  docbook-xsl-stylesheets 
BuildRequires:  po4a
BuildRequires:  libxslt-tools
BuildRequires:  zip
BuildRequires:  mingw%{_mingw_bitsize}-filesystem >= 23
BuildRequires:  mingw%{_mingw_bitsize}-cross-gcc
BuildRequires:  mingw%{_mingw_bitsize}-cross-gcc-c++
BuildRequires:  mingw%{_mingw_bitsize}-cross-binutils
BuildRequires:  mingw32-cross-nsis
BuildRequires:  mingw%{_mingw_bitsize}-wxWidgets-3_0-lang
BuildRequires:  mingw%{_mingw_bitsize}-wxWidgets-3_0-devel
BuildRequires:  mingw%{_mingw_bitsize}-jack
BuildRequires:  mingw%{_mingw_bitsize}-jack-devel
BuildRequires:  mingw%{_mingw_bitsize}-libgnurx0
BuildRequires:  mingw%{_mingw_bitsize}-libgnurx-devel
URL:            http://sourceforge.net/projects/ourorgan/
License:        GPL-2.0+
Group:          Productivity/Multimedia/Sound/Midi
Autoreqprov:    on
Version:        0.3.1.0
Release:        1
Epoch:          0
%if %{with_asio}
Source10:       asio.cpp
Source11:       asio.h
Source12:       asiodrivers.cpp
Source13:       asiodrivers.h
Source14:       asiodrvr.h
Source15:       asiolist.cpp
Source16:       asiolist.h
Source17:       asiosys.h
Source18:       combase.h
Source19:       ginclude.h
Source20:       iasiodrv.h
%endif

Source:         grandorgue-%{version}.tar.gz

BuildArch:      noarch
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Recommends:     mingw%{_mingw_bitsize}-grandorgue-demo

%description
 GrandOrgue is a virtual pipe organ sample player application supporting
 a HW1 compatible file format.

 This package contains the mingw%{_mingw_bitsize} build.

%package demo
Summary:        GrandOrgue demo sampleset
Group:          Productivity/Multimedia/Sound/Midi

%description demo
This package contains the demo sampleset for GrandOrgue

%package installer
Summary:        GrandOrgue installer
Group:          Productivity/Multimedia/Sound/Midi

%description installer
 GrandOrgue is a virtual pipe organ sample player application supporting
 a HW1 compatible file format.

 This package contains the mingw%{_mingw_bitsize} installer.

%prep
%setup -q -n grandorgue-%{version}
%if %{with_asio}
cp %{S:10} ext/rt/asio/include
cp %{S:11} ext/rt/asio/include
cp %{S:12} ext/rt/asio/include
cp %{S:13} ext/rt/asio/include
cp %{S:14} ext/rt/asio/include
cp %{S:15} ext/rt/asio/include
cp %{S:16} ext/rt/asio/include
cp %{S:17} ext/rt/asio/include
cp %{S:18} ext/rt/asio/include
cp %{S:19} ext/rt/asio/include
cp %{S:20} ext/rt/asio/include
%define ASIO    ON
%else
%define ASIO    OFF
%endif

%build
%{__mkdir} build-tools
cd build-tools
cmake ../src/build
make
cd ..
%define gooptions -DUNICODE=1 -DRTAUDIO_USE_ASIO=%ASIO  -DIMPORT_EXECUTABLES=../build-tools/ImportExecutables.cmake -DVERSION_REVISION="`echo %{version}|cut -d. -f4`" -DMSYS=1 -DSTATIC=1
%define goinstall GrandOrgue-%{version}-%{release}-win%{_mingw_bitsize}.exe
%{__mkdir} build-mingw
cd build-mingw
%{_mingw_cmake} .. -DINSTALL_DEPEND=OFF %{gooptions}
make %{?_smp_mflags} VERBOSE=1
cd ..
mkdir build-installer
cd build-installer
%{_mingw_cmake} .. -DINSTALL_DEPEND=ON  %{gooptions}
make %{?_smp_mflags} VERBOSE=1 package
cd ..

%install
rm -rf %{buildroot}
cd build-mingw
make install DESTDIR=%{buildroot}
cd ..
mkdir -p %{buildroot}
%{__cp} build-installer/GrandOrgue-%{version}-win32.exe %{buildroot}/%{_mingw_bindir}/%{goinstall}
if [ -d %{_topdir}/OTHER ]; then %{__cp} %{buildroot}/%{_mingw_bindir}/%{goinstall} %{_topdir}/OTHER ; fi

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_mingw_bindir}/*[A-Za-z].exe
%{_mingw_datadir}/locale
%dir %{_mingw_datadir}/GrandOrgue
%{_mingw_datadir}/GrandOrgue/help
%{_mingw_datadir}/GrandOrgue/sounds

%files demo
%defattr(-,root,root)
%{_mingw_datadir}/GrandOrgue/demo

%files installer
%defattr(-,root,root)
%{_mingw_bindir}/%{goinstall}

%changelog
* Tue Nov 15 2011 - martin.koegler@chello.at
- creation
