#include "PathRestrictions.h"
#include <QDir>
#include <QFileInfo>
#include <QStringList>

namespace {
QString normalizePath(const QString& path) {
    QFileInfo info(path);
    const QString canonical = info.canonicalFilePath();
    if (!canonical.isEmpty()) {
        return QDir::cleanPath(canonical);
    }
    return QDir::cleanPath(info.absoluteFilePath());
}

bool isPrefixMatch(const QString& path, const QString& prefix) {
    return path == prefix || path.startsWith(prefix + "/");
}
}

bool isRestrictedPath(const QString& path) {
    if (path.isEmpty()) {
        return false;
    }

    const QString normalized = normalizePath(path);
    const QString tempRoot = normalizePath(QDir::tempPath());
    if (isPrefixMatch(normalized, tempRoot)) {
        return false;
    }

    static const QStringList restrictedPrefixes = {
            "/System",
            "/Library",
            "/bin",
            "/sbin",
            "/usr",
            "/etc",
            "/var",
            "/private",
            "/Applications"
    };

    for (const auto& prefix : restrictedPrefixes) {
        if (isPrefixMatch(normalized, prefix)) {
            return true;
        }
    }

    return false;
}
