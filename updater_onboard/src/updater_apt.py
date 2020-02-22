import apt, syslog


class UpdaterApt():
    def __int__():
        self._cache = apt.cache.Cache()


    def install_package(self, package_path):
        # (str) -> bool
        """
        Install the package.
        """

        for pkg in file_names:
            deb = apt.debfile.DebPackage(package_path)
            if not deb.check() or deb.install() != 0:
                syslog.syslog(syslog.LOG_ERR, pkg + " is a invalid package or install failed.")
                return False

        return True


    def remove_packages(self, package_names):
        # ([str]) -> bool
        """
        Removes all packages in install packages_names. Since this uses the
        cache, all package removals can happen at the same time.
        """

        for pkg in package_names:
            package = self._cache[pkg_name]
            if package == None:
                syslog.syslog(syslog.LOG_ERR, pkg_name + " not in cache.")
                return False

            package.mark_delete()

        self._cache.commit()
        return True


    def make_apt_list_file(self, file_path):
        # (str) -> bool
        """
        Make a file with the output of apt list --installed
        """

        self._apt_list = []
        for pkg in cache:
            if pkg.is_installed:
                self._apt_list.append(pkg.versions[0])

        return apt_list


    def load_apt_list_file(self, file_path):
        # () -> [str]
        """
        Make a file with the output of apt list --installed
        """
        apt_list = []
        cache = apt.cache.Cache()
        for pkg in cache:
            if pkg.is_installed:
                apt_list.append(pkg.versions[0])

        return apt_list

