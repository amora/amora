Name: amora
Version: 0.9svn
Release: 1
Summary: A mobile remote assistant
License: GPLv2
Group: System/X11
URL: http://code.google.com/p/amora/
# svn contains some important fixes since 0.9
# (2007-11-12)
Source: http://amora.googlecode.com/files/amora_server-%{version}.tar.gz
Packager: Ademar de Souza Reis Jr. <ademar@ademar.org>

# build-requirements for Mandriva Linux, please
# adapt to your favorite distribution
BuildRequires: libimlib2-devel
BuildRequires: libbluez-devel
BuildRequires: libx11-devel
BuildRequires: libxtst-devel

BuildRoot: %{_tmppath}/%{name}-%{version}

%description
Amora is an application that enables you to control your PC desktop using a
cellphone. It uses bluetooth to send mouse and keyboard events to the
graphical session. With it you can control your slides in OpenOffice.org,
movies or any other application. Amora also has a screenshot feature, where
you can see a thumbnail in the cellphone screen of the currently focused
window in your desktop.

In order to use amora, you need a mobile phone with amora-client
installed and running. The current client is implemented in Python
for S60 (Nokia cellphones) and is available at
http://code.google.com/p/amora/

%prep
%setup -q -n amora_server-%{version}

%build
%configure
%make

%install
%{__rm} -rf %{buildroot}
%makeinstall

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root)
%doc README
%{_bindir}/amora
%{_mandir}/man1/amora.1.*

