{
  stdenv,
  lib,
  fetchFromGitHub,
  makeDesktopItem,
  cmake,
  wrapQtAppsHook,
  qtEnv,
  qt6,
}:

stdenv.mkDerivation rec {
  pname = "drawy";
  version = "1.0.0";

  src = fetchFromGitHub {
    owner = "Prayag2";
    repo = pname;
    rev = "1.0.0-alpha";
    sha256 = "sha256-5hv6iBTXTXwsmtjzRA+dAIzx/5jtWcOEfORMK0l1DSk=";
  };

  nativeBuildInputs = [
    cmake
    wrapQtAppsHook
  ];

  desktopItem = makeDesktopItem {
    name = "Drawy";
    exec = "drawy";
    icon = "drawy";
    comment = meta.description;
    desktopName = "Drawy";
    genericName = "Drawy";
    categories = [ "Graphics" ];
  };

  buildInputs = [
    qtEnv
    qt6.qtbase
  ];

  installPhase = ''
    runHook preInstall

    install -Dm755 drawy $out/bin/drawy

    for desktop_file in "$desktopItem/share/applications/*"; do
      install -Dm644 $desktop_file $out/share/applications/$(basename $desktop_file)
    done

    install -Dm644 ${./assets/logo-16.png} $out/share/icons/hicolor/16x16/apps/drawy.png
    install -Dm644 ${./assets/logo-32.png} $out/share/icons/hicolor/32x32/apps/drawy.png
    install -Dm644 ${./assets/logo-256.png} $out/share/icons/hicolor/256x256/apps/drawy.png
    install -Dm644 ${./assets/logo-512.png} $out/share/icons/hicolor/512x512/apps/drawy.png
    install -Dm644 ${./assets/logo.svg} $out/share/icons/hicolor/scalable/apps/drawy.svg

    runHook postInstall
  '';

  meta = with lib; {
    description = "Your handy, infinite, brainstorming tool";
    maintainers = with maintainers; [ quarterstar ];
    homepage = "https://github.com/Prayag2/drawy";
    license = licenses.gpl3;
    platforms = platforms.linux;
  };
}
