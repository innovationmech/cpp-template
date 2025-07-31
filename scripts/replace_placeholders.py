#!/usr/bin/env python3
"""
Placeholder Replacement Script for cpp-template

This script replaces all 'cpp-template' placeholders in the project with
a user-specified project name. It handles various file types and ensures
consistent naming throughout the project.

Usage:
    python scripts/replace_placeholders.py <new_project_name>
    
Example:
    python scripts/replace_placeholders.py my-awesome-project

Features:
- Replaces project names in CMakeLists.txt files
- Updates documentation (README.md, etc.)
- Handles source code comments and strings
- Updates directory and file names if needed
- Preserves file permissions and structure
- Creates backup of original files

Author: cpp-template project
Version: 1.0.0
"""

import os
import sys
import re
import shutil
import argparse
from pathlib import Path
from typing import List, Dict, Tuple


class PlaceholderReplacer:
    """Handles replacement of cpp-template placeholders with new project name."""
    
    def __init__(self, project_root: Path, new_name: str, backup: bool = True):
        self.project_root = project_root
        self.new_name = new_name
        self.backup = backup
        self.backup_dir = project_root / ".backup_placeholders"
        
        # Validate project name
        self._validate_project_name(new_name)
        
        # Files to process (relative to project root)
        self.files_to_process = [
            "CMakeLists.txt",
            "README.md",
            "vcpkg.json",
            "tests/CMakeLists.txt",
            "libs/CMakeLists.txt",
            "src/CMakeLists.txt",
            "examples/CMakeLists.txt",
            "docs/CUSTOMIZATION_GUIDE.md",
            "examples/usage_scenarios.cpp"
        ]
        
        # Replacement patterns
        self.replacements = self._generate_replacements()
    
    def _validate_project_name(self, name: str) -> None:
        """Validate the new project name."""
        if not name:
            raise ValueError("Project name cannot be empty")
        
        if not re.match(r'^[a-zA-Z][a-zA-Z0-9_-]*$', name):
            raise ValueError(
                "Project name must start with a letter and contain only "
                "letters, numbers, hyphens, and underscores"
            )
        
        if len(name) > 50:
            raise ValueError("Project name is too long (max 50 characters)")
    
    def _generate_replacements(self) -> List[Tuple[str, str]]:
        """Generate list of (old, new) replacement pairs."""
        replacements = []
        
        # Basic project name replacements
        replacements.append(("cpp-template", self.new_name))
        replacements.append(("cpp_template", self.new_name.replace("-", "_")))
        
        # Uppercase variants
        replacements.append(("CPP-TEMPLATE", self.new_name.upper()))
        replacements.append(("CPP_TEMPLATE", self.new_name.replace("-", "_").upper()))
        
        # CamelCase variants
        camel_case = self._to_camel_case(self.new_name)
        replacements.append(("CppTemplate", camel_case))
        
        # Namespace variants
        namespace_name = self.new_name.replace("-", "_").lower()
        replacements.append(("cpp_template", namespace_name))
        
        # URL and path replacements
        replacements.append(
            ("https://github.com/your-username/cpp-template", 
             f"https://github.com/your-username/{self.new_name}")
        )
        
        return replacements
    
    def _to_camel_case(self, name: str) -> str:
        """Convert kebab-case or snake_case to CamelCase."""
        components = re.split(r'[-_]', name)
        return ''.join(word.capitalize() for word in components)
    
    def create_backup(self) -> None:
        """Create backup of files before replacement."""
        if not self.backup:
            return
        
        print(f"Creating backup in {self.backup_dir}...")
        
        if self.backup_dir.exists():
            shutil.rmtree(self.backup_dir)
        
        self.backup_dir.mkdir()
        
        for file_path in self.files_to_process:
            full_path = self.project_root / file_path
            if full_path.exists():
                backup_path = self.backup_dir / file_path
                backup_path.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(full_path, backup_path)
                print(f"  Backed up: {file_path}")
    
    def process_file(self, file_path: Path) -> bool:
        """Process a single file for placeholder replacement."""
        if not file_path.exists():
            print(f"  Warning: File not found: {file_path}")
            return False
        
        try:
            # Read file content
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            original_content = content
            
            # Apply replacements
            for old, new in self.replacements:
                content = content.replace(old, new)
            
            # Write back if changed
            if content != original_content:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"  Updated: {file_path.relative_to(self.project_root)}")
                return True
            else:
                print(f"  No changes: {file_path.relative_to(self.project_root)}")
                return False
                
        except Exception as e:
            print(f"  Error processing {file_path}: {e}")
            return False
    
    def process_all_files(self) -> Dict[str, int]:
        """Process all files for placeholder replacement."""
        print(f"Replacing 'cpp-template' with '{self.new_name}'...")
        
        stats = {"processed": 0, "updated": 0, "errors": 0}
        
        for file_path in self.files_to_process:
            full_path = self.project_root / file_path
            stats["processed"] += 1
            
            if self.process_file(full_path):
                stats["updated"] += 1
        
        return stats
    
    def update_directory_structure(self) -> None:
        """Update directory names that contain cpp-template."""
        print("Checking for directory names to update...")
        
        # Check include directory
        include_old = self.project_root / "include" / "cpp-template"
        include_new = self.project_root / "include" / self.new_name
        
        if include_old.exists() and not include_new.exists():
            include_old.rename(include_new)
            print(f"  Renamed directory: include/cpp-template -> include/{self.new_name}")
        
        # Update any references to the old include path in CMakeLists.txt files
        cmake_files = list(self.project_root.rglob("CMakeLists.txt"))
        for cmake_file in cmake_files:
            try:
                with open(cmake_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                old_include_ref = f"include/cpp-template"
                new_include_ref = f"include/{self.new_name}"
                
                if old_include_ref in content:
                    content = content.replace(old_include_ref, new_include_ref)
                    with open(cmake_file, 'w', encoding='utf-8') as f:
                        f.write(content)
                    print(f"  Updated include path in: {cmake_file.relative_to(self.project_root)}")
                    
            except Exception as e:
                print(f"  Error updating {cmake_file}: {e}")
    
    def generate_summary_report(self, stats: Dict[str, int]) -> None:
        """Generate a summary report of the replacement process."""
        print("\n" + "=" * 60)
        print("PLACEHOLDER REPLACEMENT SUMMARY")
        print("=" * 60)
        print(f"Project name changed from 'cpp-template' to '{self.new_name}'")
        print(f"Files processed: {stats['processed']}")
        print(f"Files updated: {stats['updated']}")
        print(f"Errors encountered: {stats['errors']}")
        
        if self.backup:
            print(f"Backup created in: {self.backup_dir}")
        
        print("\nReplacements applied:")
        for old, new in self.replacements:
            print(f"  '{old}' -> '{new}'")
        
        print("\nNext steps:")
        print("1. Review the changes and test the build")
        print("2. Update any additional project-specific references")
        print("3. Update the project URL in CMakeLists.txt if needed")
        print("4. Commit the changes to version control")
        
        if self.backup:
            print(f"5. Remove backup directory when satisfied: rm -rf {self.backup_dir}")
    
    def run(self) -> None:
        """Execute the complete placeholder replacement process."""
        print(f"Starting placeholder replacement for project: {self.new_name}")
        print(f"Working directory: {self.project_root}")
        
        try:
            # Create backup
            self.create_backup()
            
            # Process files
            stats = self.process_all_files()
            
            # Update directory structure
            self.update_directory_structure()
            
            # Generate report
            self.generate_summary_report(stats)
            
            print("\nPlaceholder replacement completed successfully!")
            
        except Exception as e:
            print(f"\nError during replacement process: {e}")
            if self.backup and self.backup_dir.exists():
                print(f"Backup available in: {self.backup_dir}")
            sys.exit(1)


def main():
    """Main entry point for the script."""
    parser = argparse.ArgumentParser(
        description="Replace cpp-template placeholders with new project name",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python scripts/replace_placeholders.py my-project
  python scripts/replace_placeholders.py MyAwesomeLib --no-backup
  python scripts/replace_placeholders.py game-engine --project-root /path/to/project
"""
    )
    
    parser.add_argument(
        "project_name",
        help="New project name (letters, numbers, hyphens, underscores only)"
    )
    
    parser.add_argument(
        "--project-root",
        type=Path,
        default=Path.cwd(),
        help="Project root directory (default: current directory)"
    )
    
    parser.add_argument(
        "--no-backup",
        action="store_true",
        help="Skip creating backup of original files"
    )
    
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show what would be changed without making changes"
    )
    
    args = parser.parse_args()
    
    # Validate project root
    if not args.project_root.exists():
        print(f"Error: Project root directory does not exist: {args.project_root}")
        sys.exit(1)
    
    # Check if this looks like a cpp-template project
    main_cmake = args.project_root / "CMakeLists.txt"
    if not main_cmake.exists():
        print(f"Error: No CMakeLists.txt found in {args.project_root}")
        print("This doesn't appear to be a cpp-template project.")
        sys.exit(1)
    
    # Check if project already uses the target name
    with open(main_cmake, 'r') as f:
        cmake_content = f.read()
    
    if f"project({args.project_name}" in cmake_content:
        print(f"Warning: Project already appears to use name '{args.project_name}'")
        response = input("Continue anyway? (y/N): ")
        if response.lower() != 'y':
            print("Aborted.")
            sys.exit(0)
    
    if args.dry_run:
        print("DRY RUN MODE - No files will be modified")
        backup = False
    else:
        backup = not args.no_backup
    
    # Create and run replacer
    replacer = PlaceholderReplacer(
        project_root=args.project_root,
        new_name=args.project_name,
        backup=backup
    )
    
    if args.dry_run:
        print("\nWould apply these replacements:")
        for old, new in replacer.replacements:
            print(f"  '{old}' -> '{new}'")
        print(f"\nWould process {len(replacer.files_to_process)} files.")
    else:
        replacer.run()


if __name__ == "__main__":
    main()