
"""
    Script that can generate simple 3-D phantom volumes
    and stores these as dicom directories.
"""
import datetime
import numpy as np
import os
import pydicom
import time
import SimpleITK as sitk
from pydicom.uid import ImplicitVRLittleEndian
from pydicom.dataset import Dataset, FileDataset
import tempfile

def write_dicom(file_path, volume):
    if not os.path.exists(file_path):
        os.makedirs(file_path)

    volume = volume.astype(np.uint16)
    for slice_idx in range(volume.shape[0]):
        pixel_array = volume[slice_idx]
        img = sitk.GetImageFromArray(pixel_array)
        sitk.WriteImage(img, os.path.join(
            file_path, "{:06d}.dcm".format(slice_idx)))

def check_dicoms(file_path):
    for file_name in os.listdir(file_path):
        full_path = os.path.join(file_path, file_name)
        ds = pydicom.dcmread(full_path)
        print(ds)
        print(ds.pixel_array.shape)


if __name__ == "__main__":
    N = 16
    arr = np.zeros((N, N, N))
    for i in range(N):
        for j in range(N):
            for k in range(N):
                off = 8
                if abs(i-off) == 2 or abs(j-off) == 2 or abs(k-off) == 2:
                    arr[i,j,k] = 100

    write_dicom("../volumes/phantom", arr)

    check_dicoms("../volumes/phantom")
